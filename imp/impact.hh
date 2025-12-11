// IMPACT

#ifndef IMPACT_HH
#define IMPACT_HH

#include "../os/win/window.hh"
#include "console.hh"
#include "game.hh"
#include "server.hh"
#include "scene/title.hh"
#include "scene/planet2d.hh"
#include "entity/mine.hh"

// Last login credentials saved in this file
//! encrypted
const str LOGIN_FILE = "../data/login.dat";
// Font options located in gl/fonts
const vec<str> FONTS = {"aldo"};

// Global object that keeps the game, interface, and network in sync.
// Inputs route through here
struct Impact : virtual window, virtual Console {

  // Set in execution file to select the interface
  enum { UI_CONSOLE, UI_WINDOW } UI_MODE;

  // Impact game
  Game game;
  // Network
  Server serv;

  // Title scene displayed first upon execution
  Title scene_title;
  // Overhead view of a planet
  Planet2D scene_planet2d;

  // If not NULL, signals an entity can be placed
  Entity* build_pending;

  Impact();

  virtual void validate(const str& func);
  virtual void init();
  virtual void run();
  virtual void update(const double ms);

  vec<Location::LocationLevel> scene_options() const;

  scene* select_scene();
  void process_key(env::key_event ke); };

// Set default member state
Impact::Impact(): build_pending(NULL) {
  type = "Impact"; }

// Ensure valid state
void Impact::validate(const str& func){
  if(UI_MODE == UI_CONSOLE) Console::validate(func);
  else window::validate(func);
  game.validate(func);
  serv.validate(func); }

// Prepare both the game and interface to run
// Called by: win_exec:wWinMain, exec:main
void Impact::init(){
  serv.init();
  // Initialize console, otherwise window environment
  if(UI_MODE == UI_CONSOLE){
    Console::init();
    return; }
  // Load fonts
  for(int i = 0; i < FONTS.size(); ++i)
    scene::fonts[FONTS[i]] = font(FONTS[i]);
  // Initialize env and title scene
  scene_title.init();
  window::init(); }

// Start the game and interface
// Called by: win_exec:wWinMain, exec:main
void Impact::run(){
  serv.run();
  // Attempt login from saved credentials
  ifstream fs(LOGIN_FILE);
  assert(fs.is_open(), "Impact.init", "login file couldn't open");
  str user, pass;
  getline(fs, user);
  getline(fs, pass);
  fs.close();
  serv.login(user, pass);
  // Run console
  if(UI_MODE == UI_CONSOLE){
    Console::run();
    return; }
  // Otherwise run window environment
  scene_title.run();
  window::run(); }

// Update everything. Called continuously in a loop.
// Called by: window.main_loop
void Impact::update(const double ms){
  // Handle all key events since last update
  while(!keys.empty()){
    env::key_event ke = keys.front();
    keys.pop();
    process_key(ke); }
  // Update server, game, env -> scenes
  serv.update(ms);
  game.update(ms);
  window::update(ms);
  // Update scene with server login status
  if(scene_title.logged_in == false && serv.user_id != 0)
    scene_title.logged_in = true;
  last_update = clock();
  validate("Impact.update"); }

// Determine all the scenes the user can view
// Called by: select_scene
vec<Location::LocationLevel> Impact::scene_options() const {
  vec<Location::LocationLevel> r;
  if(game.player->loc->level == Location::INSTANCE){
    r.pb(Location::INSTANCE);
    return r; }
  if(game.player->loc->level == Location::ENTITY || game.player->ship != NULL)
    r.pb(Location::ENTITY);
  if(game.player->loc->level == Location::PLANET)
    r.pb(Location::PLANET);
  if(game.player->loc->level <= Location::SYSTEM && game.player->stage >= 5)
    r.pb(Location::SYSTEM);
  if(game.player->loc->level <= Location::SECTOR && game.player->stage >= 6)
    r.pb(Location::SECTOR);
  if(game.player->loc->level <= Location::GALAXY && game.player->stage >= 7)
    r.pb(Location::GALAXY);
  if(game.player->loc->level <= Location::CLUSTER && game.player->stage >= 8)
    r.pb(Location::CLUSTER);
  if(game.player->stage >= 9)
    r.pb(Location::UNIVERSE);
  return r; }

// Select the best scene to view
// Called by: PlayBtn.click
scene* Impact::select_scene(){
  // If no scenes are active, switch to title scene
  bool dead = true;
  for(pair<llu, scene*> s : scenes)
    if(s.second->active == true){
      dead = false;
      break; }
  if(dead){
    scene_title.run();
    return &scene_title; }
  // Select lowest level scene besides your ship
  vec<Location::LocationLevel> opts = scene_options();
  Location::LocationLevel dest = Location::UNIVERSE;
  if(opts.size() == 1 && opts[0] == Location::ENTITY)
    dest = Location::ENTITY;
  for(int i = 0; i < opts.size(); ++i)
    if(opts[i] < dest && opts[i] != Location::ENTITY)
      dest = opts[i];
  scene* r = NULL;
  switch(dest){
  //! handle other scenes
  case Location::PLANET:
    r = &scene_planet2d;
    break;
  default:
    err("select_scene error"); }
  validate("Impact.select_scene");
  return r; }

// Handle key events sent through the window
// Called by: update
void Impact::process_key(env::key_event ke){
  if(ke.key == "LCLICK" && ke.down)
    click(ke.cursor);
  else if(ke.key == "M" && ke.down && build_pending == NULL)
    build_pending = new Mine();
  else if(ke.key[0] >= '0' && ke.key[0] <= '9'){
    vec<Location::LocationLevel> opts = scene_options();
    int k = ke.key[0] - '0';
    //! switch to scenes if contains(opts, LocationLevel)
  }
  validate("Impact.process_key"); }

#endif

// IMPACT

#ifndef IMPACT_HH
#define IMPACT_HH

#include "../os/win/window.hh"
#include "console.hh"
#include "game.hh"
#include "server.hh"
#include "scene/title.hh"
#include "scene/planet2d.hh"

// Last login credentials saved in this file
//! encrypted
const str LOGIN_FILE = "data/login.dat"
// Font options located in gl/fonts
const vec<str> FONTS = {"aldo"};

// Set in execution file to select the interface
enum { UI_CONSOLE, UI_WINDOW } UI_MODE;

// Global object that keeps the game, interface, and network in sync.
// Inputs route through here
struct Impact : virtual window, virtual Console, virtual Game, virtual Server {

  // Title scene displayed first upon execution
  Title scene_title;
  // Overhead view of a planet
  Planet2D scene_planet2d;

  Impact();

  virtual void validate(str func);
  virtual void init();
  virtual void run();
  virtual void update(double ms);

  void process_key();
  vec<Location::LocationLevel> scene_options();
  void select_scene(); };

// Called by: global
Impact::Impact(): type("Impact") {}

// Ensure valid state
void Impact::validate(str func){
  if(UI_MODE == UI_CONSOLE) Console::validate(func);
  else window::validate(func);
  Game::validate(func);
  Server::validate(func); }

// Prepare both the game and interface to run
// Called by: win_exec:wWinMain, exec:main
void Impact::init(){
  // Initialize console, otherwise window environment
  if(UI_MODE == UI_CONSOLE){
    console::init();
    return; }
  // Attempt login from saved credentials
  ifstream fs(LOGIN_FILE);
  assert(fs.is_open(), "login file couldn't open");
  str user, pass;
  getline(fs, user);
  getline(fs, pass);
  fs.close();
  scene_title.logged_in = login(user, pass);
  // Load fonts
  for(int i = 0; i < FONTS.size(); ++i)
    scene::fonts[FONTS[i]] = font(FONTS[i]);
  // Initialize window, env, and title scene
  scene_title.init();
  window::init();
  validate("impact.init"); }

// Start the game and interface
// Called by: win_exec:wWinMain, exec:main
void Impact::run(){
  // Run console
  if(UI_MODE == UI_CONSOLE){
    console::run();
    return; }
  // Otherwise run window environment
  scene_title.run();
  window::run();
  validate("impact.run"); }

// Update everything. Called continuously in a loop.
// Called by: window.main_loop
void Impact::update(){
  system::update();
  // Handle all key events since last update
  while(!e.keys.empty()){
    env::key_event kp = e.keys.front();
    e.keys.pop();
    process_key(kp); }
  // Update server, game, env->scenes
  Server::update();
  Game::update();
  env::update();
  last_update = clock();
  validate("impact.update"); }

// Handle key events sent through the window
// Called by: update
void Impact::process_key(env::key_event kp){
  if(kp.key == "LCLICK")
    click(kp.cursor);
  else if(kp.key[0] >= '0' && kp.key[0] <= '9'){
    vec<Location::LocationLevel> opts = scene_options();
    int k = kp.key[0] - '0';
    //! switch to scenes if contains(opts, LocationLevel)
  }
  validate("impact.process_key"); }

// Determine all the scenes the user can view
// Called by: select_scene
vec<Location::LocationLevel> Impact::scene_options(){
  vec<Location::LocationLevel> r;
  if(player->loc->level == Location::INSTANCE){
    r.pb(Location::INSTANCE);
    return r; }
  if(player->loc->level == Location::ENTITY || player->ship != NULL)
    r.pb(Location::ENTITY);
  if(player->loc->level == Location::PLANET)
    r.pb(Location::PLANET);
  if(player->loc->level <= Location::SYSTEM && player->stage >= 5)
    r.pb(Location::SYSTEM);
  if(player->loc->level <= Location::SECTOR && player->stage >= 6)
    r.pb(Location::SECTOR);
  if(player->loc->level <= Location::GALAXY && player->stage >= 7)
    r.pb(Location::GALAXY);
  if(player->loc->level <= Location::CLUSTER && player->stage >= 8)
    r.pb(Location::CLUSTER);
  if(player->stage >= 9)
    r.pb(Location::UNIVERSE);
  validate("impact.scene_options");
  return r; }

// Select the best scene to view
// Called by: PlayBtn.click
scene* Impact::select_scene(){
  // If no scenes are active, switch to title scene
  bool dead = true;
  for(scene* s : scenes)
    if(s->active == true){
      dead = false;
      break; }
  if(dead){
    scene_title.run();
    return; }
  // Select lowest level scene besides your ship
  vec<Location::LocationLevel> opts = scene_options();
  Location::LocationLevel dest = Location::UNIVERSE;
  if(opts.size() == 1 && opts[0] == Location::ENTITY)
    dest = Location::ENTITY;
  for(int i = 0; i < opts.size(); ++i)
    if(opts[i] < dest && opts[i] != Location::ENTITY)
      dest = opts[i];
  scene* r;
  switch(dest){
  //! handle other scenes
  case Location::PLANET:
    r = &scene_planet2d;
    break;
  default:
    err("select_scene error"); }
  validate("impact.select_scene");
  return r; }

#endif

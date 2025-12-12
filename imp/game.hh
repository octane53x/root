// GAME

#ifndef GAME_HH
#define GAME_HH

#include "entity/unit/bot.hh"

// How often most the game is updated, an exception being object positions
// that need to be updated more often to be displayed properly
const double GAME_TICK = 0.1; // seconds

struct Game;

// Set in constructor. Enables locations to add themselves.
Game* _game;

// The local game containing locations and entities, receiving data from the
// server through the global Impact object that derives this
struct Game : virtual system {

  // Time between game updates
  clock_t tick;
  // All locations the player can currently have information on
  umap<llu, Location*> locs;
  // Unit being directly controlled by the user
  Bot* player;

  Game();

  virtual void validate(const str& func);
  virtual void init();
  virtual void update(const double ms); };

// Set default member state
// Called by: global
Game::Game(): player(NULL) {
  type = "Game";
  _game = this; }

// Add to Game.locations
Location::Location(){
  _game->locs[id] = this; }

// Ensure valid state
void Game::validate(const str& func){
  system::validate(func);
  if(active){
    assert(player != NULL, func, "Game.player is NULL");
    player->validate(func); } }

//! Temporary logic
// Called by: PlayBtn.click
void Game::init(){
  system::init();
  tick = (clock_t)ceil(GAME_TICK * CLOCKS_PER_SEC);

  // Create planet
  Planet* planet = new Planet();
  planet->init();

  // Place player on land
  //! only works for box land
  player = new Bot(planet);
  bool found = false;
  while(!found){
    player->pos.x = (double)(lrand() % planet->size);
    player->pos.y = (double)(lrand() % planet->size);
    for(int i = 0; i < planet->terrain.land.size(); ++i){
      point topleft, botright;
      topleft = botright = planet->terrain.land[i].points[0];
      for(int j = 1; j < planet->terrain.land[i].points.size(); ++j){
        point p = planet->terrain.land[i].points[j];
        if(p.x < topleft.x || p.y < topleft.y)
          topleft = p;
        if(p.x > botright.x || p.y > botright.y)
          botright = p; }
      if(player->pos.x >= topleft.x && player->pos.x <= botright.x
          && player->pos.y >= topleft.y && player->pos.y <= botright.y){
        found = true; break; } } }
  debug("Player spawned at "+player->pos.to_str());

  // Load chunks around player
  Chunk* chunk = planet->find_chunk(player->pos);
  planet->terrain.gen_chunk(chunk);
  if(chunk->N != NULL) planet->terrain.gen_chunk(chunk->N);
  if(chunk->S != NULL) planet->terrain.gen_chunk(chunk->S);
  if(chunk->W != NULL) planet->terrain.gen_chunk(chunk->W);
  if(chunk->E != NULL) planet->terrain.gen_chunk(chunk->E);
  if(chunk->N->W != NULL) planet->terrain.gen_chunk(chunk->N->W);
  if(chunk->N->E != NULL) planet->terrain.gen_chunk(chunk->N->E);
  if(chunk->S->W != NULL) planet->terrain.gen_chunk(chunk->S->W);
  if(chunk->S->E != NULL) planet->terrain.gen_chunk(chunk->S->E); }

// Update the game's locations, which also updates the entities within
// Called by: Impact.update
void Game::update(const double ms){
  system::update(ms);
  clock_t now = clock();
  if(now - last_update >= tick){
    for(pair<llu, Location*> loc : locs){
      loc.second->update_game();
      for(pair<llu, Entity*> ent : loc.second->entities)
        ent.second->update_game(); }
    last_update = clock(); }
  validate("Game.update"); }

#endif

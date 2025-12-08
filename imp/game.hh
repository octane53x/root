// GAME

#ifndef GAME_HH
#define GAME_HH

#include "planet/planet.hh"
#include "unit/bot.hh"

// How often most the game is updated, an exception being object positions
// that need to be updated more often to be displayed properly
const double GAME_TICK = 0.1; // seconds

// The local game containing locations and entities, receiving data from the
// server through the global Impact object that derives this
struct Game : system {

  // Time between game updates
  clock_t tick;
  // All locations the player can currently have information on
  umap<llu, Location*> locs;
  // Unit being directly controlled by the user
  Bot* player;

  Game();

  virtual void validate(const str& func) const;
  virtual void init();
  virtual void run();
  virtual void update(const double ms); };

// Set default member state
// Called by: global
Game::Game(): player(NULL) {}

// Ensure valid state
void Game::validate(const str& func) const {
  system::validate(func);
  player->validate(func);
  assert(player != NULL, "Game.player is NULL"); }

//! Temporary logic
// Called by: PlayBtn.click
void Game::init(){
  system::init();
  tick = (clock_t)ceil(GAME_TICK * CLOCKS_PER_SEC);

  // Create planet
  Planet* planet = new Planet();
  planet->init();

  // Place player on land
  player = new Bot();
  player->loc = planet;
  bool found = false;
  while(!found){
    player->pos.x = lrand() % planet->size;
    player->pos.y = lrand() % planet->size;
    for(int i = 0; i < planet->terrain.land.size(); ++i)
      if(planet->terrain.land[i].inside(player->pos)){
        found = true; break; } }

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
  if(chunk->S->E != NULL) planet->terrain.gen_chunk(chunk->S->E);
  validate("Game.init"); }

// Update the game's locations, which also updates the entities within
// Called by: Impact.update
void Game::update(const double ms){
  clock_t now = clock();
  if(now - last_update >= tick){
    for(pair<llu, Location*> loc : locs)
      loc.second->update_game();
    last_update = clock(); }
  validate("Game.update"); }

#endif

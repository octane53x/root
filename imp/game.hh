// GAME

#ifndef GAME_HH
#define GAME_HH

#include "update.hh"

struct Game {
  clock_t last_update, tick;
  Planet* planet; // Where the player is
  Bot* player;

  Game(){}
  void init(){
    last_update = 0;
    tick = GAME_TICK * CLOCKS_PER_SEC;
    //! Connect to server
    //! Generate or find planet
  }

  void update(){
    clock_t now = clock();
    if(now - last_update >= tick){
      if(planet != NULL) planet->update();
      last_update = clock(); } }

  // Player actions
  void move(int x, int y);
  void build(Feature* f); };

void Game::move(int x, int y){
  if(planet->surface[x][y].feature != NULL){
    printf("ERR: Tile occupied\n");
    return; }
  sleep(1000);
  int xp = (int)floor(player->loc.x), yp = (int)floor(player->loc.y);
  vec<Unit*>& u = planet->surface[xp][yp].units;
  for(int i = 0; i < u.size(); ++i){
    if(u[i]->id == player->id){
      Bot* b = (Bot*)u[i];
      u.erase(u.begin() + i);
      planet->surface[x][y].units.pb(b);
      b->loc = point(x, y);
      break; } }
  printf("Moved player from (%d,%d) to (%d,%d)\n", xp,yp,x,y); }

void Game::build(Feature* f){
  for(int x = (int)floor(f->loc.x); x < (int)ceil(f->loc.x + f->size.x); ++x)
    for(int y = (int)floor(f->loc.y); y < (int)ceil(f->loc.y + f->size.y); ++y)
      if(planet->surface[x][y].feature != NULL){
        printf("ERR: Tile occupied\n");
        return; }
  planet->features.pb(f);
  for(int x = (int)floor(f->loc.x); x < (int)ceil(f->loc.x + f->size.x); ++x)
    for(int y = (int)floor(f->loc.y); y < (int)ceil(f->loc.y + f->size.y); ++y)
      planet->surface[x][y].feature = f; }

#endif

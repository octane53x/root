// GAME

#ifndef GAME_HH
#define GAME_HH

#include "../lang/lang.hh"
#include "planet.hh"

struct Game {
  int last_id;
  Planet* planet; // Where the player is
  Bot* player;
  Game(): last_id(1) {
    planet = new Planet();
    player = new Bot();
    int x = PLAYER_START_X, y = PLAYER_START_Y;
    planet->surface[x][y].units.pb(player);
    player->loc.x = x, player->loc.y = y; }
  int next_id(){
    return ++last_id; }
  // Player actions
  void move(int x, int y);
  void build(Feature f); };


void Game::move(int x, int y){
  if(planet->surface[x][y].feature != NULL){
    printf("ERR: Tile occupied\n");
    return; }
  sleep(1000);
  const vec<Unit>& u = planet->surface[xp][yp].units;
  int xp = player->loc.x, yp = player->loc.y;
  for(int i = 0; i < u.size(); ++i){
    if(u[i].id = player->id){
      Bot b = (Bot)u[i];
      u.erase(u.begin() + i);
      u.pb(b);
      break; } }
  printf("Moved player from (%d,%d) to (%d,%d)\n", xp,yp,x,y); }

void Game::build(Feature* f){
  for(int x = f->x; x < f->x+f->size.x; ++x)
    for(int y = f->y; y < f->y+f->size.y; ++y)
      if(planet->surface[x][y].feature != NULL){
        printf("ERR: Tile occupied\n");
        return; }
  planet->features.pb(f);
  for(int x = f->x; x < f->x+f->size.x; ++x)
    for(int y = f->y; y < f->y+f->size.y; ++y)
      planet->surface[x][y].feature = f; }

#endif

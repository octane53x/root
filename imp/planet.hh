// PLANET

#ifndef PLANET_HH
#define PLANET_HH

#include "feature.hh"
#include "unit.hh"

struct Block : thing {
  map<str, int> minerals;
  vec<Block*> tunnels;
  Block(){ type = "Block"; } };

struct Tile : thing {
  int altitude;
  str name;
  Feature* feature;
  vec<Unit*> units;
  Tile(){ type = "Tile"; } };

struct Planet : thing {
  int id;
  vec<vec<vec<Block> > > earth;
  vec<vec<Tile> > surface;
  vec<Feature*> features;
  Planet(){
    type = "Planet";
    gen_earth();
    gen_surface(); }
  void gen_earth();
  void gen_surface(); };

void Planet::gen_earth(){
  // Custom minerals
  for(int i = 0; i < MINE_WIDTH; ++i){
    earth.pb(vec<vec<Block> >());
    for(int j = 0; j < MINE_WIDTH; ++j){
      earth[i].pb(vec<Block>());
      for(int k = 0; k < MINE_DEPTH; ++k){
        earth[i][j].pb(Block());
        earth[i][j][k].minerals["SOIL"] = 100;
        if(!(rand() % 4))
          earth[i][j][k].minerals["STONE"] = rand() % 100 + 1; } } }

  // Random minerals
  // vec<str> used; //! uset
  // int M = rand() % (MAX_MINERALS - MIN_MINERALS) + MIN_MINERALS;
  // for(int m = 0; m < M; ++m){

  //   // Select mineral
  //   int r;
  //   bool done = false;
  //   while(!done){
  //     r = rand() % (MINERALS.size()-C_MINERALS) + C_MINERALS;
  //     if(!contains(used, MINERALS[r]))
  //       done = true, used.pb(MINERALS[r]); }

    // Place mineral
    //! int B =
  //}
}

void Planet::gen_surface(){
  for(int i = 0; i < SURFACE_WIDTH; ++i){
    surface.pb(vec<Tile>());
    for(int j = 0; j < SURFACE_WIDTH; ++j){
      Tile t;
      t.altitude = 0;
      t.name = "GRASSLAND";
      surface[i].pb(t); } } }

#endif

// PLANET

#ifndef PLANET_HH
#define PLANET_HH

#include "feature.hh"
#include "unit.hh"

struct Block {
  bool tunnel;
  double progress;
  point loc;
  map<str, int> minerals;
  vec<Block*> adjacent;

  Block(){}
  int total(){
    int r = 0;
    map<str, int>::iterator it;
    for(it = minerals.begin(); it != minerals.end(); ++it)
      r += it->second;
    return r; } };

struct Tile {
  point loc;
  str type;
  Feature* feature;
  vec<Unit*> units;
  Tile(){} };

struct Planet {
  int id;
  vec<vec<vec<Block> > > earth;
  vec<vec<Tile> > surface;
  vec<Feature*> features;
  vec<Unit*> units;

  Planet(){
    gen_earth();
    gen_surface(); }
  void update(){
    for(Feature* f : features)
      f->update(this);
    for(Unit* u : units)
      u->update(this); }

  void gen_earth();
  void gen_surface(); };

void Planet::gen_earth(){
  // Custom minerals
  for(int i = 0; i < MINE_WIDTH; ++i){
    earth.pb(vec<vec<Block> >());
    for(int j = 0; j < MINE_WIDTH; ++j){
      earth[i].pb(vec<Block>());
      for(int k = 0; k < MINE_DEPTH; ++k){
        Block b;
        b.loc = point(i, j, k);
        earth[i][j].pb(b);
        earth[i][j][k].minerals["SOIL"] = 100;
        if(!(rand() % 4))
          earth[i][j][k].minerals["STONE"] = rand() % 100 + 1; } } }

  // Set adjacent blocks
  for(int i = 0; i < MINE_WIDTH; ++i)
    for(int j = 0; j < MINE_WIDTH; ++j)
      for(int k = 0; k < MINE_DEPTH; ++k){
        Block* b = &earth[i][j][k];
        if(i > 0) b->adjacent.pb(&earth[i-1][j][k]);
        if(i < MINE_WIDTH-1) b->adjacent.pb(&earth[i+1][j][k]);
        if(j > 0) b->adjacent.pb(&earth[i][j-1][k]);
        if(j < MINE_WIDTH-1) b->adjacent.pb(&earth[i][j+1][k]);
        if(k > 0) b->adjacent.pb(&earth[i][j][k-1]);
        if(k < MINE_DEPTH-1) b->adjacent.pb(&earth[i][j][k+1]); }

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
      t.loc = point(i, j, 0); //! altitude
      t.type = "GRASSLAND";
      surface[i].pb(t); } } }

#endif

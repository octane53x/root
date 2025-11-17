// PLANET

#ifndef PLANET_HH
#define PLANET_HH

#include "../feature/feature.hh"
#include "../unit/unit.hh"
#include "terrain.hh"
#include "earth.hh"

const int
    PLANET_SIZE_MIN = 10000,
    PLANET_SIZE_MAX = 1000000,
    CHUNK_SIZE = 1000;

enum Direction { NORTH, SOUTH, EAST, WEST };

struct Planet : object {

  int size; // Side length of square surface
  Terrain terrain;
  Earth earth;
  vec<vec<Chunk> > chunks;
  umap<llu, Feature*> features;
  umap<llu, Unit*> units;

  Planet(){}

  void init(){
    size = (rand() % ((PLANET_SIZE_MAX - PLANET_SIZE_MIN) / CHUNK_SIZE + 1))
        * CHUNK_SIZE + PLANET_SIZE_MIN;
    int nchunks = size / CHUNK_SIZE;
    for(int i = 0; i < nchunks; ++i){
      chunks.pb(vec<Chunk>());
      for(int j = 0; j < nchunks; ++j){
        Chunk c;
        c.size = CHUNK_SIZE;
        c.seed = rand();
        chunks[i].pb(c); } }
    terrain.gen_planet(size); //!! Time
    earth.gen_planet(size); }

  virtual void validate(){
    object::validate(); }

  virtual point update(){ return point(0, 0); }

  virtual void draw(image* bkgd){}

  void game_update(){
    for(pair<llu, Feature*> f : features)
      f.second->game_update(this);
    for(pair<llu, Unit*> u : units)
      u.second->game_update(this); } };

#endif

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

struct Planet : object {

  int size; // Side length of square surface
  Terrain terrain;
  Earth earth;
  vec<vec<Chunk> > chunks; // chunks[i][j]: i runs N to S, j runs W to E
  umap<llu, Feature*> features;
  umap<llu, Unit*> units;

  Planet(){}

  virtual void validate(){
    assert(size >= PLANET_SIZE_MIN, "planet.size incorrect");
    object::validate(); }

  void init(){
    size = (lrand() % ((PLANET_SIZE_MAX - PLANET_SIZE_MIN) / CHUNK_SIZE + 1))
        * CHUNK_SIZE + PLANET_SIZE_MIN;
    // Create chunks
    int nchunks = size / CHUNK_SIZE;
    double x = 0.0;
    for(int i = 0; i < nchunks; ++i){
      chunks.pb(vec<Chunk>());
      double y = 0.0;
      for(int j = 0; j < nchunks; ++j){
        Chunk c;
        c.pos = point(x, y);
        c.size = CHUNK_SIZE;
        c.seed = (ui)lrand();
        chunks[i].pb(c);
        y += CHUNK_SIZE; }
      x += CHUNK_SIZE; }
    // Connect chunks
    for(int i = 0; i < chunks.size(); ++i)
      for(int j = 0; j < chunks[i].size(); ++j){
        if(i > 0) chunks[i][j].N = &chunks[i-1][j];
        if(i < nchunks-1) chunks[i][j].S = &chunks[i+1][j];
        if(j > 0) chunks[i][j].W = &chunks[i][j-1];
        if(j < nchunks-1) chunks[i][j].E = &chunks[i][j+1]; }
    // Generate planet
    terrain.gen_planet(size); //!! Time
    earth.gen_planet(size); }

  Chunk* find_chunk(point p){
    int i = (int)floor(p.y / CHUNK_SIZE);
    int j = (int)floor(p.x / CHUNK_SIZE);
    return &chunks[i][j]; }

  //! Incomplete
  void move_unit(Unit* unit, Tile* dest){
    unit->path.clear();
    point p(unit->tile->pos.x + 0.5, unit->tile->pos.y + 0.5);
    while(dlt(p.x, dest->pos.x + 0.5))
      p.x += 1.0, unit->path.push(p);
    while(dgt(p.x, dest->pos.x + 0.5))
      p.x -= 1.0, unit->path.push(p);
    while(dlt(p.y, dest->pos.y + 0.5))
      p.y += 1.0, unit->path.push(p);
    while(dgt(p.y, dest->pos.y + 0.5))
      p.y -= 1.0, unit->path.push(p); }

  virtual point update(double ms){ return point(0, 0); }

  virtual void draw(image* bkgd, viewport view){}

  void game_update(){
    for(pair<llu, Feature*> f : features)
      f.second->game_update(this);
    for(pair<llu, Unit*> u : units)
      u.second->game_update(this); } };

#endif

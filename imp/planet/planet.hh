// PLANET

#ifndef PLANET_HH
#define PLANET_HH

#include "../feature/feature.hh"
#include "../unit/unit.hh"
#include "terrain.hh"
#include "earth.hh"

const int
    MIN_PLANET_SIZE = 10000,
    MAX_PLANET_SIZE = 1000000,
    CHUNK_SIZE = 100;

enum Direction { NORTH, SOUTH, EAST, WEST };

struct Planet : object {

  int size; // Side length of square surface
  umap<llu, Chunk*> chunks; // Loaded chunks based on camera position
  umap<llu, Feature*> features;
  umap<llu, Unit*> units;

  Planet(){}

  void init(){
    size = (rand() % ((MAX_PLANET_SIZE - MIN_PLANET_SIZE) / CHUNK_SIZE + 1))
        * CHUNK_SIZE + MIN_PLANET_SIZE;
    //!
  }

  virtual void validate(){
    object::validate(); }

  virtual point update(){ return point(0, 0); }

  virtual void draw(image* bkgd){}

  void game_update(){
    for(pair<llu, Feature*> f : features)
      f.second->game_update(this);
    for(pair<llu, Unit*> u : units)
      u.second->game_update(this); }

  void load_chunk(Chunk* src, Direction dir){
    Chunk* chunk = new Chunk();
    chunk->size = CHUNK_SIZE;
    switch(dir){
      case NORTH:
        chunk->pos = point(src->pos.x, src->pos.y + CHUNK_SIZE);
        break;
      case SOUTH:
        chunk->pos = point(src->pos.x, src->pos.y - CHUNK_SIZE);
        break;
      case EAST:
        chunk->pos = point(src->pos.x + CHUNK_SIZE, src->pos.y);
        break;
      case WEST:
        chunk->pos = point(src->pos.x - CHUNK_SIZE, src->pos.y);
        break;
      default: break;
    }
    Terrain().generate(id, chunk);
    Earth().generate(id, chunk);
  }

  void delete_chunk(Chunk* chunk){
    //!
  } };

#endif

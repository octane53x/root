// TERRAIN GENERATOR

#ifndef TERRAIN_HH
#define TERRAIN_HH

#include "chunk.hh"

struct Terrain : thing {

  virtual void validate(){}

  void generate(llu rng_seed, Chunk* chunk){
    //!
  } };

#endif

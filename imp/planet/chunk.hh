// CHUNK

#ifndef CHUNK_HH
#define CHUNK_HH

#include "block.hh"
#include "tile.hh"

// Chunk size defined in planet.hh

struct Chunk : object {

  int size;
  vec<vec<vec<Block> > > earth;
  vec<vec<Tile> > terrain;

  Chunk(){}

  virtual void validate(){}

  virtual point update(double ms){ return point(0, 0); }

  virtual void draw(image* bkgd){} };

#endif

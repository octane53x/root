// CHUNK

#ifndef CHUNK_HH
#define CHUNK_HH

#include "tile.hh"

// Chunk size defined in planet.hh

struct Chunk : object {

  int size;
  ui seed;
  vec<vec<Tile> > tiles;

  Chunk(){}

  virtual void validate(){}

  virtual point update(double ms){ return point(0, 0); }

  virtual void draw(image* bkgd, viewport view){} };

#endif

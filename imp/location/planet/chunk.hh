// CHUNK

// Chunk size defined in planet.hh

#ifndef CHUNK_HH
#define CHUNK_HH

#include "tile.hh"

// A 1x1 Terrain Tile, defining the surface of a Planet
// Generated on the order of N^2, so storage size is kept minimal
enum Tile : uchar { WATER, GRASS, SNOW, CLIFF, BUSH, COAST, SWAMP };

struct Chunk : object {

  int size;
  ui seed;
  vec<vec<Tile> > tiles; // tiles[i][j]: i runs N to S, j runs W to E
  Chunk* N, *S, *E, *W;

  Chunk(): N(NULL), S(NULL), E(NULL), W(NULL) {}

  virtual void validate(){}

  Tile find_tile(point p){
    assert(p.x >= pos.x && p.x <= pos.x + size
        && p.y >= pos.y && p.y <= pos.y + size,
        "Chunk.find_tile point not in chunk");
    int i = (int)floor(p.y - pos.y), j = (int)floor(p.x - pos.x);
    return tiles[i][j]; }

  virtual point update(double ms){ return point(0, 0); }

  virtual void draw(image* bkgd, viewport view){} };

#endif

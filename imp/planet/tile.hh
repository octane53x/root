// PLANET TILE

#ifndef TILE_HH
#define TILE_HH

#include "../feature/feature.hh"
#include "../unit/unit.hh"

struct Tile {

  point loc;
  str type;
  Feature* feature;
  vec<Unit*> units;

  Tile(){} };

#endif

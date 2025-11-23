// PLANET TILE

#ifndef TILE_HH
#define TILE_HH

#include "../feature/feature.hh"
#include "../unit/unit.hh"

struct Tile : object {

  enum TileType {
    WATER,
    GRASS,
    SNOW,
    CLIFF,
    BUSH,
    COAST,
    SWAMP };

  TileType type;
  Feature* feature;
  vec<Unit*> units;

  Tile(){}

  virtual void validate(){}

  virtual point update(double ms){ return point(0, 0); }

  virtual void draw(image* bkgd, viewport view){} };

#endif

// BOT UNIT

#ifndef BOT_HH
#define BOT_HH

#include "unit.hh"

struct Bot : Unit {

  enum Target {
    FEATURE,
    UNIT,
    TILE };

  int stage;
  Target target_type;
  Feature* feature_target;
  Unit* unit_target;
  Tile* tile_target;

  Bot(): stage(3), type(BOT), speed(0.5) {}

  virtual void validate(){
    Unit::validate();
    str msg("bot stage and location incompatible");
    switch(stage){
    case 1:
      assert(loc->level == Location::INSTANCE, msg);
      break;
    case 2:
      assert(loc->level == Location::ENTITY, msg);
      break;
    case 3:
    case 4:
      assert(loc->level <= Location::PLANET, msg);
      break;
    case 5:
      assert(loc->level <= Location::SYSTEM, msg);
      break;
    case 6:
      assert(loc->level <= Location::SECTOR, msg);
      break;
    case 7:
      assert(loc->level <= Location::GALAXY, msg);
      break;
    case 8:
      assert(loc->level <= Location::CLUSTER, msg);
      break;
    case 9:
    case 10:
      break;
    default:
      err("player stage outside bounds"); } }

  virtual void game_update(Planet* p){
    //!
  }

  virtual point update(double ms){ return point(0, 0); } };

#endif

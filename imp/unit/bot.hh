// BOT UNIT

#ifndef BOT_HH
#define BOT_HH

#include "unit.hh"

struct Bot : Unit {

  enum Target {
    FEATURE,
    UNIT,
    TILE };

  Target target_type;
  Feature* feature_target;
  Unit* unit_target;
  Tile* tile_target;

  Bot(){ type = BOT; }

  virtual void validate(){
    Unit::validate(); }

  virtual void game_update(Planet* p){
    //!
  }

  virtual point update(double ms){ return point(0, 0); } };

#endif

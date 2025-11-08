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
    Unit::validate(); } };

#endif

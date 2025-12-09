// BOT UNIT

#ifndef BOT_HH
#define BOT_HH

#include "unit.hh"

struct Bot : virtual Unit {

  int stage;
  point target_point;
  Entity* target;

  Bot(): stage(3) {
    type = "Bot";
    speed = 0.5; }

  virtual void validate(const str& func){
    Unit::validate(func);
    str msg("bot stage and location incompatible");
    switch(stage){
    case 1:
      assert(loc->level == Location::INSTANCE, msg); break;
    case 2:
      assert(loc->level == Location::ENTITY, msg); break;
    case 3:
    case 4:
      assert(loc->level <= Location::PLANET, msg); break;
    case 5:
      assert(loc->level <= Location::SYSTEM, msg); break;
    case 6:
      assert(loc->level <= Location::SECTOR, msg); break;
    case 7:
      assert(loc->level <= Location::GALAXY, msg); break;
    case 8:
      assert(loc->level <= Location::CLUSTER, msg); break;
    case 9:
    case 10:
      break;
    default:
      err("player stage outside bounds"); } }

  virtual void draw(image* canvas, const viewport& view){
    //! draw
  }

  virtual void update_game(){
    //! recharge weapon, etc
  }
};

#endif

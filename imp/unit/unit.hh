// ABSTRACT UNIT

#ifndef UNIT_HH
#define UNIT_HH

#include "../../gl/obj/point.hh"

struct Planet;

enum UnitType {
  WORKER,
  BOT,
  SHIP
};

struct Unit {

  UnitType type;
  llu id;
  static llu next_id;

  int health, speed, energy;
  point loc;

  Unit(){
    id = new_id();
    set_type(); }

  llu new_id(){ return next_id++; }

  virtual void set_type() = 0;

  virtual void update(Planet* p) = 0; };

llu Unit::next_id = 1;

#endif

// SHIP

#ifndef SHIP_HH
#define SHIP_HH

#include "unit.hh"

struct Ship : virtual Unit {

  umap<llu, Unit*> units;

  Ship();

  virtual void validate();
  virtual void update();
};

Ship::Ship(): type("Ship") {}

void Ship::validate(){
  Unit::validate(); }

void Ship::update(){
  Unit::update(); }

#endif

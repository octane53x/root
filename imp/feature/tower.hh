// TOWER FEATURE

#ifndef TOWER_HH
#define TOWER_HH

#include "../planet/planet.hh"

struct Tower : Feature {

  int damage, range;
  Unit* target;

  Tower(){ type = TOWER; }

  virtual void validate(){
    Feature::validate(); }

  void update(Planet* p){
    //!
  } };

#endif

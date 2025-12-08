// FACTORY FEATURE

#ifndef FACTORY_HH
#define FACTORY_HH

#include "../planet/planet.hh"
#include "../mech/recipe.hh"

// One product per factory at a time
struct Factory : Feature {

  Recipe recipe;

  Factory(){ type = FACTORY; }

  virtual void validate(){
    Feature::validate(); }

  virtual void game_update(Planet* p){
    //!
  }

  virtual point update(double ms){ return point(0, 0); } };

#endif

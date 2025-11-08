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

  void update(Planet* p){
    //!
  } };

#endif

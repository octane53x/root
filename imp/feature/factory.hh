// FACTORY FEATURE

#ifndef FACTORY_HH
#define FACTORY_HH

#include "../planet/planet.hh"
#include "../mech/recipe.hh"

struct Factory : Feature {

  Recipe recipe;
  map<str, int> gems;

  Factory(int _x, int _y){
    loc = point(_x, _y);
    size = point(FACTORY_SIZE, FACTORY_SIZE); }

  void set_type(){ type = FeatureType::FACTORY; }

  void update(Planet* p){
    //!
  } };

#endif

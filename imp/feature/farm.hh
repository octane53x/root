// FARM FEATURE

#ifndef FARM_HH
#define FARM_HH

#include "../planet/planet.hh"

struct Farm : Feature {

  graph<point> border;
  vec<vec<str> > crops;
  vec<Worker*> workers;
  map<str, int> gems;

  Farm(){}

  void set_type(){ type = FeatureType::FARM; }

  void update(Planet* p){
    //!
  } };

#endif

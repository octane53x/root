// FARM FEATURE

#ifndef FARM_HH
#define FARM_HH

#include "../planet/planet.hh"

struct Farm : Feature {

  vec<vec<Item> > crops;
  vec<Worker*> workers; //?

  Farm(){ type = FARM; }

  virtual void validate(){
    Feature::validate(); }

  void update(Planet* p){
    //!
  } };

#endif

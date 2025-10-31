// LABORATORY FEATURE

#ifndef LAB_HH
#define LAB_HH

#include "../planet/planet.hh"

struct Lab : Feature {

  map<str, int> gems;

  Lab(){}

  void set_type(){ type = FeatureType::LAB; }

  void update(Planet* p){
    //!
  } };

#endif

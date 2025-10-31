// TREE FEATURE

#ifndef TREE_HH
#define TREE_HH

#include "../planet/planet.hh"

struct Tree : Feature {

  int wood;

  Tree(){}

  void set_type(){ type = FeatureType::TREE; }

  void update(Planet* p){
    //!
  } };

#endif

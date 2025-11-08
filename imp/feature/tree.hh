// TREE FEATURE

#ifndef TREE_HH
#define TREE_HH

#include "../planet/planet.hh"

struct Tree : Feature {

  int wood;

  Tree(){ type = TREE; }

  virtual void validate(){
    Feature::validate(); }

  void update(Planet* p){
    //!
  } };

#endif

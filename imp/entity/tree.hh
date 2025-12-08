// TREE FEATURE

#ifndef TREE_HH
#define TREE_HH

#include "../planet/planet.hh"

struct Tree : Feature {

  int wood;

  Tree(){ type = TREE; }

  virtual void validate(){
    Feature::validate(); }

  virtual void game_update(Planet* p){
    //!
  }

  virtual point update(double ms){ return point(0, 0); } };

#endif

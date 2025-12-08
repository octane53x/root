// LABORATORY FEATURE

#ifndef LAB_HH
#define LAB_HH

#include "../planet/planet.hh"

struct Lab : Feature {

  //! Research

  Lab(){ type = LAB; }

  virtual void validate(){
    Feature::validate(); }

  virtual void game_update(Planet* p){
    //!
  }

  virtual point update(double ms){ return point(0, 0); } };

#endif

// LABORATORY FEATURE

#ifndef LAB_HH
#define LAB_HH

#include "../planet/planet.hh"

struct Lab : Feature {

  //! Research

  Lab(){ type = LAB; }

  virtual void validate(){
    Feature::validate(); }

  void update(Planet* p){
    //!
  } };

#endif

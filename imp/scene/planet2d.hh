// 2D Planet View

#ifndef PLANET2D_HH
#define PLANET2D_HH

#include "../../gl/scene.hh"

struct Planet2D : scene {

  Planet2D(){}

  virtual void validate(){
    scene::validate(); }

  void init(int w, int h){
    scene::init(w, h);

    //!
  }

  void update(double ms){} };

#endif

// 2D Planet View

#ifndef PLANET2D_HH
#define PLANET2D_HH

#include "../../gl/scene.hh"

struct Planet2D : scene {

  Planet* planet;

  Planet2D(){}

  virtual void validate(){
    scene::validate(); }

  // Requires planet to be initialized
  void init(int w, int h){
    bkgd_color = BLUE;
    scene::init(w, h);
    view.size = planet->size;

    // Draw land
    for(int i = 0; i < planet->terrain.land.size(); ++i){
      polygon* land = &planet->terrain.land[i];
      land->fill = YELLOW;
      objs.pb(land); } }

  void update(double ms){} };

#endif

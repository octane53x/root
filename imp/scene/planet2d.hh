// 2D Planet View

#ifndef PLANET2D_HH
#define PLANET2D_HH

#include "../../gl/scene.hh"

struct Planet2D : scene {

  Planet* planet;

  Planet2D(){}

  virtual void validate(){
    scene::validate(); }

  void init(int w, int h){
    bkgd_color = BLUE;
    scene::init(w, h);

    // Draw land
    for(int i = 0; i < planet->terrain.land.size(); ++i){
      polygon* land = &planet->terrain.land[i];
      polygon* land2 = new polygon();
      for(int j = 0; j < land->points.size(); ++j){
        point p = land->points[j];
        double ratio = (double)height / planet->size;
        land2->points.pb(point(p.x * ratio, p.y * ratio)); }
      land2->fill = YELLOW;
      objs.pb(land2); } }

  void update(double ms){} };

#endif

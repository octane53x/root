// 2D Planet View

#ifndef PLANET2D_HH
#define PLANET2D_HH

#include "../../gl/scene.hh"

// Overhead view of the planet surface
struct Planet2D : virtual scene {

  // Planet in view
  Planet* planet;

  Planet2D();

  virtual void init(); };

// Set default member state
Planet2D::Planet2D(){
  type = "Planet2D"; }

// Display the surface around the player
// Requires planet to be initialized
void Planet2D::init(){
  bkgd_color = BLUE;
  scene::init();
  view.size = planet->size;
  // Draw land
  for(int i = 0; i < planet->terrain.land.size(); ++i){
    polygon* land = &planet->terrain.land[i];
    land->fill = YELLOW;
    objs.pb(land); } }

#endif

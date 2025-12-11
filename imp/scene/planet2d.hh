// 2D Planet View

#ifndef PLANET2D_HH
#define PLANET2D_HH

#include "../../gl/scene.hh"

// Overhead view of the planet surface
struct Planet2D : virtual scene {

  // Planet in view
  Planet* planet;
  // Player unit
  Bot* player;

  Planet2D();

  virtual void init();
  virtual void draw(image* canvas, const viewport& view); };

// Set default member state
Planet2D::Planet2D(){
  type = "Planet2D"; }

// Display the surface around the player
// Requires planet to be initialized
// Called by: PlayBtn.click_fn
void Planet2D::init(){
  width = win_w, height = win_h;
  bkgd_color = BLUE;
  // Draw land
  for(int i = 0; i < planet->terrain.land.size(); ++i){
    polygon* land = &planet->terrain.land[i];
    land->fill = YELLOW;
    objs[land->id] = land; }
  scene::init();
  view.size_in = planet->size; }

// Draw onto image
void Planet2D::draw(image* canvas, const viewport& view){
  scene::draw(canvas, view);
  // Draw player here so it's the same size no matter the planet zoom
  polygon poly;
  double ratio = (double)min(canvas->width, canvas->height) / planet->size;
  int x = (int)floor(player->pos.x * ratio);
  int y = (int)floor(player->pos.y * ratio);
  poly.add(point(x - 10, y - 10));
  poly.add(point(x + 10, y - 10));
  poly.add(point(x + 10, y + 10));
  poly.add(point(x - 10, y + 10));
  poly.fill = RED;
  viewport default_view;
  default_view.size_in = default_view.size_out =
      min(canvas->width, canvas->height);
  poly.draw(canvas, default_view); }

#endif

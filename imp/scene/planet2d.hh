// 2D Planet View

#ifndef PLANET2D_HH
#define PLANET2D_HH

#include "../../gl/scene.hh"

const double
    // Coordinate size of viewport over player
    VIEW_SIZE = 100.0;

// Overhead view of the planet surface
struct Planet2D : virtual scene {

  // Button over a tile to detect clicks
  struct TerrainBtn : virtual button {
    virtual void hover_fn();
    virtual void click_fn();
  } terrain_btn;

  // Planet in view
  Planet* planet;
  // Player unit
  Bot* player;

  Planet2D();

  virtual void init();
  virtual void update(const double ms);
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
  objs[player->id] = player;

  terrain_btn.fill = CLEAR;
  terrain_btn.add(point(0, 0));
  terrain_btn.add(point(planet->size, 0));
  terrain_btn.add(point(planet->size, planet->size));
  terrain_btn.add(point(0, planet->size));
  objs[terrain_btn.id] = &terrain_btn;

  scene::init();
  vp.size_in = VIEW_SIZE; }

// Adjust the viewport to follow player
void Planet2D::update(const double ms){
  scene::update(ms);
  vp.top_left =
      point(player->pos.x - VIEW_SIZE/2.0, player->pos.y - VIEW_SIZE/2.0); }

// Draw onto image
void Planet2D::draw(image* canvas, const viewport& view){
  img = bkgd;
  // Draw tiles in view
  vec<Chunk*> chunks = planet->find_chunk(player->pos)->neighbors();
  for(double x = vp.top_left.x; x <= vp.top_left.x + vp.size_in; x += 1.0)
    for(double y = vp.top_left.y; y <= vp.top_left.y + vp.size_in; y += 1.0)
      for(int i = 0; i < chunks.size(); ++i)
        if(chunks[i]->in_chunk(point(x, y))){
          point tl(floor(x), floor(y));
          point br(tl.x + 1.0, tl.y + 1.0);
          int x0 = (int)floor(vp.translate_out(tl).x);
          int xf = (int)floor(vp.translate_out(br).x);
          int y0 = (int)floor(vp.translate_out(tl).y);
          int yf = (int)floor(vp.translate_out(br).y);
          color c = TILE_TEX[*chunks[i]->find_tile(point(x, y))];
          for(int xx = x0; xx <= xf; ++xx)
            for(int yy = y0; yy <= yf; ++yy)
              bkgd.set_pixel(xx, yy, c);
          break; }
  scene::draw(canvas, view);
  validate("Planet2D.draw"); }

#endif

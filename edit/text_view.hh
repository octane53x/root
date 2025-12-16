// TEXT VIEW SCENE

#ifndef TEXT_VIEW_HH
#define TEXT_VIEW_HH

#include "../gl/scene.hh"

struct TextView : virtual scene {

  TextView();

  virtual void init();
  virtual void update(const double ms); };

TextView::TextView(){
  type = "TextView"; }

void TextView::init(){
  width = win_w, height = win_h;
  bkgd_color = BLACK;

  //! init

  scene::init(); }

void TextView::update(const double ms){
  bkgd_color = color().random();
  for(int i = 0; i < bkgd.height; ++i)
    for(int j = 0; j < bkgd.width; ++j)
      bkgd.set_pixel(j, i, bkgd_color); }

#endif

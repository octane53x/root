// SCENE

#ifndef SCENE_HH
#define SCENE_HH

#include "gl_incl.hh"
#include "ui.hh"

struct Scene {
  point win_size;
  color bkgd_color;
  Frame bkgd;
  vec<Label> labels;
  vec<Button> buttons;

  Scene(){}
  virtual Frame next_frame() = 0;
  virtual void draw_bkgd(){
    Frame f;
    for(int i = 0; i < win_size.x; ++i){
      f.pb(vec<color>());
      for(int j = 0; j < win_size.y; ++j)
        f[i].pb(bkgd_color); }
    bkgd = f; } };

#endif

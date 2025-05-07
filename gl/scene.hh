// SCENE

#ifndef SCENE_HH
#define SCENE_HH

#include "gl_incl.hh"
#include "ui.hh"

struct Scene {
  color bkgd_color;
  Frame bkgd;
  vec<Label> labels;
  vec<Button> buttons;
  virtual Frame next_frame() = 0;

  Scene(){}
  Frame draw_bkgd(){
    Frame f;
    for(int i = 0; i < WIN_W; ++i){
      f.data.pb(vec<color>());
      for(int j = 0; j < WIN_H; ++j)
        f.data.pb(bkgd_color);
    }
  } };

#endif

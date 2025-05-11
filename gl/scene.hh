// SCENE

#ifndef SCENE_HH
#define SCENE_HH

#include "ui.hh"

struct scene {
  point win_size;
  color bkgd_color;
  image bkgd_frame;
  umap<str, font*> fonts;
  vec<label> labels;
  vec<button> buttons;

  scene(){}
  virtual image next_frame() = 0;
  virtual void draw_bkgd(){
    image f(win_size);
    for(int j = 0; j < win_size.y; ++j)
      for(int i = 0; i < win_size.x; ++i)
        f.data[j][i] = bkgd_color;
    bkgd_frame = f; } };

#endif

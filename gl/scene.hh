// SCENE

#ifndef SCENE_HH
#define SCENE_HH

#include "ui.hh"

struct scene : thing {
  point win_size;
  color bkgd_color;
  image frame;
  umap<str, font*> fonts;
  vec<label> labels;
  vec<button> buttons;

  scene(){ type = "scene"; }
  virtual void draw_bkgd(){
    if(frame.data.empty()){
      frame.size = win_size;
      for(int i = 0; i < win_size.y; ++i){
        frame.data.pb(vec<color>());
        for(int j = 0; j < win_size.x; ++j)
          frame.data[i].pb(bkgd_color); }
      return; }
    for(int i = 0; i < win_size.y; ++i)
      for(int j = 0; j < win_size.x; ++j)
        frame.data[i][j] = bkgd_color; }
  virtual image* next_frame() = 0; };

#endif

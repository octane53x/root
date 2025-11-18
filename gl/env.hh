// ENVIRONMENT

// Load scenes before initializing

#ifndef ENV_HH
#define ENV_HH

#include "scene.hh"

bool zcompare_scene(const scene* a, const scene* b){
  return a->z > b->z; }

struct env : thing {

  int win_w, win_h;
  point cursor;
  color bkgd_color;
  queue<pair<str, bool> > keys; // true = keydown
  image bkgd, frame;
  vec<scene*> scenes;

  env(): bkgd_color(BLACK) {}

  virtual void validate(){
    assert(win_w > 0 && win_h > 0, "window size not positive"); }

  void init(int w, int h){
    win_w = w, win_h = h;
    bkgd.set_size(w, h);
    for(int i = 0; i < h; ++i)
      for(int j = 0; j < w; ++j)
        bkgd.set_pixel(j, i, bkgd_color);
    for(int i = 0; i < scenes.size(); ++i)
      scenes[i]->init(w, h); }

  image* draw_scenes(){
    vec<scene*> active_scenes;
    for(int i = 0; i < scenes.size(); ++i)
      if(scenes[i]->active) active_scenes.pb(scenes[i]);
    sort(active_scenes.begin(), active_scenes.end(), zcompare_scene);
    frame = bkgd;
    for(int i = 0; i < active_scenes.size(); ++i)
      active_scenes[i]->next_frame()->draw(&frame);
    return &frame; } };

#endif

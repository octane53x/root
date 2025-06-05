// ENVIRONMENT

#ifndef ENV_HH
#define ENV_HH

#include "scene.hh"

struct env {
  int cursor_x, cursor_y, win_w, win_h;
  queue<pair<str, bool> > keys; // true = keydown
  umap<str, font> fonts;
  scene* active_scene;
  env(){}
  void init(int w, int h){
    active_scene->win_w = win_w = w, active_scene->win_h = win_h = h;
    umap<str, font>::iterator it;
    for(it = fonts.begin(); it != fonts.end(); ++it)
      active_scene->fonts[it->first] = &it->second;
  }
  virtual void update() = 0; };

#endif

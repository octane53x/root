// ENVIRONMENT

#ifndef ENV_HH
#define ENV_HH

#include "scene.hh"
#include "camera.hh"

struct env : thing {

  int cursor_x, cursor_y, win_w, win_h;
  queue<pair<str, bool> > keys; // true = keydown
  umap<str, font> fonts;
  scene* active_scene;
  camera cam;

  env(){}

  void init(int w, int h){
    win_w = w, win_h = h;
    umap<str, font>::iterator it;
    for(it = fonts.begin(); it != fonts.end(); ++it)
      active_scene->fonts[it->first] = &it->second; }

  virtual void validate(){}

  virtual void update() = 0; };

#endif

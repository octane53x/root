// ENVIRONMENT

#ifndef ENV_HH
#define ENV_HH

#include "../core/const.hh"
#include "scene.hh"

struct env {
  point cursor, win_size;
  queue<pair<str, bool> > keys; // true = keydown
  umap<str, font> fonts;
  scene* active_scene;
  env(point _win_size): win_size(_win_size) {}
  void init(){
    active_scene->win_size = win_size;
    umap<str, font>::iterator it;
    for(it = fonts.begin(); it != fonts.end(); ++it)
      active_scene->fonts[it->first] = &it->second;
    //!
  } };

#endif

// ENVIRONMENT

#ifndef ENV_HH
#define ENV_HH

#include "gl_incl.hh"
#include "scene.hh"

struct Env {
  point cursor, win_size;
  queue<pair<str, bool> > keys; // true = keydown
  umap<str, umap<char, vec<vec<color> > > > fonts;
  Scene* active_scene;
  Env(){
    win_size = point(INIT_WIN_W, INIT_WIN_H); }
  void init(){
    active_scene->win_size = win_size; } };

#endif

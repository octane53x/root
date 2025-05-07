// ENVIRONMENT

#ifndef ENV_HH
#define ENV_HH

#include "../lang/core/obj.hh"
#include "scene.hh"

struct Env {
  point cursor;
  queue<pair<str, bool> > keys; // true = keydown
  umap<str, umap<char, vec<vec<color> > > > fonts;
  Scene* active_scene;
  Env(){}
};

#endif

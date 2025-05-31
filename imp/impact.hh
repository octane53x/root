// IMPACT

#ifndef IMPACT_HH
#define IMPACT_HH

#include "../gl/font.hh"
#include "../gl/env.hh"
#include "game.hh"
#include "scene/title.hh"

struct Impact : env {
  Game* g;
  Title title;
  Impact(int _w, int _h): env(_w, _h) {}
  void init(){
    font f("aldo");
    fonts[f.name] = f;
    active_scene = &title;
    env::init();
    title.init();
    //!
    } };

#endif

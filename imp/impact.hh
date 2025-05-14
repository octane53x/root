// IMPACT

#ifndef IMPACT_HH
#define IMPACT_HH

#include "../gl/font.hh"
#include "../gl/env.hh"
#include "game.hh"
#include "scene/title.hh"

struct Impact : env {
  Game* g;
  Impact(point _win_size): env(_win_size) {}
  void init(){
    font f("aldo");
    fonts[f.name] = f;
    Title* title = new Title();
    active_scene = title;
    env::init();
    title->init();
    //!
    } };

#endif

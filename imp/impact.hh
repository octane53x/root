// IMPACT

#ifndef IMPACT_HH
#define IMPACT_HH

#include "../gl/fonts/font.hh"
#include "../gl/env.hh"
#include "game.hh"
#include "scene/title.hh"

struct Impact : env {
  Game* g;
  Impact(point _win_size): env(_win_size) {}
  void init(){
    fonts["aldo"] = input_font("aldo");
    Title* title = new Title();
    active_scene = title;
    env::init();
    title->init();
    title->draw_bkgd();
    //!
    } };

#endif

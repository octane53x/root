// IMPACT

#ifndef IMPACT_HH
#define IMPACT_HH

#include "../gl/font.hh"
#include "../gl/env.hh"
#include "game.hh"
#include "scene/title.hh"

struct Impact : env {
  Game game;
  Title title;

  Impact(){}
  void init(int w, int h){
    fonts["aldo"] = font("aldo");
    active_scene = &title;
    env::init(w, h);
    title.init();
    game.init(); }

  void update(){
    game.update();
  } };

#endif

// IMPACT

#ifndef IMPACT_HH
#define IMPACT_HH

#include "../gl/font.hh"
#include "../gl/env.hh"
#include "game.hh"
#include "scene/title.hh"

struct Impact : env {

  Game game;
  Title* title;

  Impact(){}

  virtual void validate(){
    env::validate(); }

  void init(int w, int h){
    fonts["aldo"] = font("aldo");
    title = new Title(w, h);
    active_scene = title;
    env::init(w, h);
    title->init();
    game.init(); }

  void update(){
    game.update();
  } };

#endif

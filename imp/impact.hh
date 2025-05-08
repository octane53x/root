// IMPACT

#ifndef IMPACT_HH
#define IMPACT_HH

#include "../lang/lang.hh"
#include "game.hh"
#include "scenes.hh"

struct Impact : Env {
  Game* g;
  Impact(){}
  void init(){
    active_scene = new TitleScreen();
    Env::init();
    active_scene->draw_bkgd(); } };

#endif

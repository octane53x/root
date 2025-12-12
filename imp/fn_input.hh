// INPUT FUNCTIONS

// Cannot be defined in their object files because the project depends on the
// input objects to create them, but their functions call the project

#ifndef FN_INPUT_HH
#define FN_INPUT_HH

#include "global.hh"

// Hover the play button
void Title::PlayBtn::hover_fn(){
  //! hover
}

// Play the game, leaving the title screen and loading from server
// Called by: scenes/title button
void Title::PlayBtn::click_fn(){
  imp.scene_title.load();
  imp.game.init();
  scene* s = imp.select_scene();
  if(s->type == "Planet2D"){
    Planet2D* ss = dynamic_cast<Planet2D*>(s);
    ss->planet = dynamic_cast<Planet*>(imp.game.player->loc);
    ss->player = imp.game.player;
  }else
    err("PlayBtn.click_fn", "unhandled scene");
  s->init();
  imp.game.run();
  imp.scene_title.stop();
  s->run(); }

// Hover the terrain
void Planet2D::TerrainBtn::hover_fn(){
  //! move the target reticule
}

// Click the terrain to move the player or target a position
void Planet2D::TerrainBtn::click_fn(){
  point p = imp.scene_planet2d.vp.translate_in(click_pix);
  if(imp.build_pending == NULL)
    imp.scene_planet2d.player->move(p);
  else{
    Entity* e = imp.build_pending;
    e->pos = p;
    e->init();
    e->run();
    imp.scene_planet2d.objs[e->id] = e;
    imp.build_pending = NULL; } }

#endif

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
  imp.Game::init();
  scene* s = imp.select_scene();
  s->init();
  imp.Game::run();
  imp.scene_title.stop();
  s->run(); }

#endif

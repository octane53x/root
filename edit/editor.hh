// EDITOR

#ifndef EDITOR_HH
#define EDITOR_HH

#include "../os/win/window.hh"
#include "text_view.hh"

int scene::win_w, scene::win_h;
umap<str, font> scene::fonts;

struct Editor : virtual window {

  TextView scene_text;

  Editor();

  virtual void init();
  virtual void run();
  virtual void update(const double ms);

  void process_key(env::key_event ke); };

Editor::Editor(){
  type = "Editor"; }

void Editor::init(){
  //! Load font
  scene_text.init();
  window::init(); }

void Editor::run(){
  scene_text.run();
  window::run(); }

void Editor::update(const double ms){
  while(!keys.empty()){
    env::key_event ke = keys.front();
    keys.pop();
    process_key(ke); }
  window::update(ms);
  last_update = clock(); }

void Editor::process_key(env::key_event ke){
  //! Key input
}

#endif

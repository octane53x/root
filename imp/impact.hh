// IMPACT

#ifndef IMPACT_HH
#define IMPACT_HH

#include "../gl/font.hh"
#include "../gl/env.hh"
#include "game.hh"
#include "scene/title.hh"
#include "scene/planet2d.hh"

const int
    WIN_OFFSET_X = 8,
    WIN_OFFSET_Y = 32;

clock_t start_time;
umap<str, font> scene::fonts; // static member
env e;
Title title_scene;
Planet2D planet_view;
Game game;

// --------------------------------
// Inputs
// --------------------------------

struct Play : button {
  void fn(){
    title_scene.active = false;
    planet_view.active = true; } };

void process_key(env::keypress kp){
  if(kp.key == "LCLICK")
    e.click(kp.cursor); }

// --------------------------------
// Window API
// --------------------------------

void init_impact(int w, int h){
  start_time = clock();
  // Add buttons
  title_scene.play_btn = new Play();
  e.buttons.pb(title_scene.play_btn);
  // Add scenes to env
  e.scenes.pb(&title_scene);
  e.scenes.pb(&planet_view);
  // Load fonts into scenes
  scene::fonts["aldo"] = font("aldo");
  // Initialize env and game
  title_scene.active = true;
  game.init();
  planet_view.planet = game.planet;
  e.init(w, h); }

void update_impact(){
  while(!e.keys.empty()){
    env::keypress kp = e.keys.front();
    e.keys.pop();
    process_key(kp); }
  game.camera_update(planet_view.cam.pos);
  game.update(); }

void update_window(int x, int y, int w, int h){
  e.set_window(point(x, y), w, h); }

void update_cursor(int x, int y){
  e.cursor = point(x - e.winpos.x - WIN_OFFSET_X,
      y - e.winpos.x - WIN_OFFSET_Y); }

void send_key(str key, bool down, int cursor_x, int cursor_y){
  env::keypress kp;
  kp.key = key, kp.down = down;
  kp.cursor = point((double)cursor_x - e.winpos.x - WIN_OFFSET_X,
      (double)cursor_y - e.winpos.y - WIN_OFFSET_Y);
  e.keys.push(kp); }

image* get_frame(){
  return e.draw_scenes(); }

#endif

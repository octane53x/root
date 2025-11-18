// IMPACT

#ifndef IMPACT_HH
#define IMPACT_HH

#include "../gl/font.hh"
#include "../gl/env.hh"
#include "game.hh"
#include "scene/title.hh"
#include "scene/planet2d.hh"

umap<str, font> scene::fonts; // static member
env e;
Title title_scene;
Planet2D planet_view;
Game game;

struct Play : button {
  void fn(){
    title_scene.active = false;
    planet_view.active = true; } };

void init_impact(int w, int h){
  // Add buttons
  title_scene.play = new Play();
  // Add scenes to env
  e.scenes.pb(&title_scene);
  e.scenes.pb(&planet_view);
  // Load fonts into scenes
  scene::fonts["aldo"] = font("aldo");
  // Initialize env and game
  title_scene.active = true;
  e.init(w, h);
  game.init(); }

void update_impact(){
  game.camera_update(planet_view.cam.pos);
  game.update(); }

void update_cursor(int x, int y){
  e.cursor = point(x, y); }

void send_key(str key, bool dir){
  e.keys.push(pair<str, bool>(key, dir)); }

image* get_frame(){
  return e.draw_scenes(); }

#endif

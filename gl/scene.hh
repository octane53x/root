// SCENE

#ifndef SCENE_HH
#define SCENE_HH

#include "obj.hh"

bool zcompare(const object* a, const object* b){
  return a->pos.z > b->pos.z; }

struct move_node {
  object* obj;
  move_node* parent;
  vec<move_node*> children; };

struct scene {
  clock_t last_frame;
  int win_w, win_h;
  color bkgd_color;
  image frame;
  umap<str, font*> fonts;
  vec<object*> objs;

  scene(): last_frame(0) {}
  virtual image* next_frame() = 0;

  virtual void draw_bkgd(){
    if(frame.data.empty()){
      frame.width = win_w, frame.height = win_h;
      for(int i = 0; i < win_h; ++i){
        frame.data.pb(vec<color>());
        for(int j = 0; j < win_w; ++j)
          frame.data[i].pb(bkgd_color); }
      return; }
    for(int i = 0; i < win_h; ++i)
      for(int j = 0; j < win_w; ++j)
        frame.data[i][j] = bkgd_color; }

  void move_objs(double ms);
  void move_rec(move_node* node, point mov, double ms);

  void draw_objs(){
    sort(objs.begin(), objs.end(), zcompare);
    for(int i = 0; i < objs.size(); ++i)
      objs[i]->draw(&frame);
  } };

#endif

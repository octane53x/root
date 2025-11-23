// ABSTRACT OBJECT

#ifndef OBJECT_HH
#define OBJECT_HH

#include "point.hh"
#include "viewport.hh"

struct image;

struct object : thing {

  struct movement {

    enum mov_type {
      ROOT,
      PATH,
      ORBIT,
      CUSTOM };

    mov_type type;
    int path_pos;
    double vel, path_prog; // vel = pixels/sec
    uvec axis;
    vec<point> path;
    object* root;

    movement(mov_type t):
        type(t), vel(0.0), path_pos(0), path_prog(0.0), root(NULL) {} };

  llu id;
  static llu next_id;
  point pos;
  color fill;
  movement* mov;

  object(): fill(WHITE), mov(NULL) { id = new_id(); }

  llu new_id(){ return next_id++; }

  // Returns movement
  virtual point update(double ms) = 0;

  virtual void draw(image* bkgd, viewport view) = 0;

  virtual void validate(){
    if(mov != NULL &&
        (mov->type == movement::ROOT || mov->type == movement::ORBIT))
      assert(mov->root != NULL, "movement.root not set"); }

  // mov_type::ROOT handled by scene
  point move(double ms){
    point move = point(0, 0);
    if(mov == NULL || mov->type == movement::ROOT) return move;
    if(mov->type == movement::CUSTOM) return update(ms);
    double dist = mov->vel * (ms / 1000.0);
    if(mov->type == movement::PATH){
      while(dist > 0.0){
        point p = mov->path[mov->path_pos];
        double step = p.dist(point(0, 0));
        double next = step - mov->path_prog;
        if(dist < next){
          move += p * (dist / step);
          mov->path_prog += dist;
          dist = 0.0;
        }else{
          move += p * (next / step);
          dist -= next;
          mov->path_prog = 0.0;
          ++mov->path_pos;
          if(mov->path_pos == mov->path.size()) mov->path_pos = 0; } }
    }else if(mov->type == movement::ORBIT){
      double r = pos.dist(mov->root->pos);
      double deg = dist / r;
      point p = pos;
      p.rotate(mov->root->pos, deg);
      move = p - pos; }
    pos += move;
    return move; } };

llu object::next_id = 1;

#endif

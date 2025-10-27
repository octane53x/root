// OBJECT

#ifndef OBJECT_HH
#define OBJECT_HH

#include "image.hh"
#include "model.hh"

enum mov_type {
  NONE,
  ROOT,
  PATH,
  ORBIT,
  CUSTOM };

struct object;

struct movement {

  mov_type type;
  int path_pos;
  double vel, path_prog; // vel = pixels/sec
  uvec axis;
  vec<point> path;
  object* root;

  movement(): type(mov_type::NONE), vel(0.0), path_pos(0), path_prog(0.0),
      root(NULL) {} };

struct object {

  llu id;
  static llu next_id;
  point pos, img_root;
  uvec rot;
  image img;
  model mdl;
  movement mov;

  object(){ id = next_id++; }

  // mov_type::ROOT handled by scene
  point move(double ms){
    point move = point(0, 0);
    double dist = mov.vel * (ms / 1000.0);
    if(mov.type == mov_type::NONE || mov.type == mov_type::ROOT) return move;
    if(mov.type == mov_type::CUSTOM) return update(ms);
    if(mov.type == mov_type::PATH){
      while(dist > 0.0){
        point p = mov.path[mov.path_pos];
        double step = p.dist(point(0, 0));
        double next = step - mov.path_prog;
        if(dist < next){
          move += p * (dist / step);
          mov.path_prog += dist;
          dist = 0.0;
        }else{
          move += p * (next / step);
          dist -= next;
          mov.path_prog = 0.0;
          ++mov.path_pos;
          if(mov.path_pos == mov.path.size()) mov.path_pos = 0; } }
    }else if(mov.type == mov_type::ORBIT){
      double r = pos.dist(mov.root->pos);
      double deg = dist / r;
      point p = pos;
      p.rotate(mov.root->pos, deg);
      move = p - pos; }
    pos += move;
    return move; }

  virtual point update(double ms){ return point(0, 0); }

  void draw(image* bkgd){
    for(int i = 0; i < img.height; ++i)
      for(int j = 0; j < img.width; ++j){
        int y = (int)floor(pos.y - img_root.y) + i;
        int x = (int)floor(pos.x - img_root.x) + j;
        if(img.data[i][j] != WHITE
            && y >= 0 && y < bkgd->height && x >= 0 && x < bkgd->width)
          bkgd->data[y][x] = img.data[i][j]; } } };

llu object::next_id = 1;

#endif

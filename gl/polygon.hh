// POLYGON

#ifndef POLYGON_HH
#define POLYGON_HH

#include "line.hh"
#include "image.hh"

struct polygon : object {

  vec<point> points;
  color fill;

  polygon(){}
  polygon(const vec<point>& _p){ points = _p; }

  virtual void validate(){
    object::validate(); }

  virtual point update(double ms){ return point(0, 0); }

  //!
  bool inside(point p){ return false; }

  void draw(image* bkgd){
    vec<point> points2;
    for(int i = 0; i < points.size(); ++i)
      points2.pb(point(points[i].x + pos.x, points[i].y + pos.y));
    int top = bkgd->height, bot = 0, left = bkgd->width, right = 0;
    for(int i = 0; i < points2.size(); ++i){
      top = min(top, (int)floor(points2[i].y));
      bot = max(bot, (int)ceil(points2[i].y));
      left = min(left, (int)floor(points2[i].x));
      right = max(right, (int)ceil(points2[i].x)); }
    image tmp(right-left+1, bot-top+1);
    vec<point> p;
    for(int i = 0; i < points2.size(); ++i)
      p.pb(point(points2[i].x-left, points2[i].y-top));
    for(int i = 0; i < p.size()-1; ++i)
      line(p[i], p[i+1]).draw(&tmp);
    line(p.back(), p[0]).draw(&tmp);
    int x, y = (tmp.height >> 1);
    for(x = 0; x < tmp.width  ; ++x)
      if(tmp.data[y][x] == BLACK){ ++x; break; }
    queue<point> q;
    q.push(point(x, y));
    tmp.set_pixel(x, y, BLACK);

    while(!q.empty()){
      point p = q.front();
      q.pop();
      x = (int)floor(p.x), y = (int)floor(p.y);
      if(tmp.data[y-1][x] == CLEAR){
        q.push(point(p.x, p.y-1));
        tmp.set_pixel(x, y-1, BLACK); }
      if(tmp.data[y+1][x] == CLEAR){
        q.push(point(p.x, p.y+1));
        tmp.set_pixel(x, y+1, BLACK); }
      if(tmp.data[y][x-1] == CLEAR){
        q.push(point(p.x-1, p.y));
        tmp.set_pixel(x-1, y, BLACK); }
      if(tmp.data[y][x+1] == CLEAR){
        q.push(point(p.x+1, p.y));
        tmp.set_pixel(x+1, y, BLACK); } }
    for(int i = 0; i < tmp.height; ++i)
      for(int j = 0; j < tmp.width; ++j)
        if(tmp.data[i][j] == BLACK)
          bkgd->set_pixel(j+left, i+top, fill); } };

#endif

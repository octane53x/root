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

  //! Add object.pos to points
  void draw(image* bkgd){
    int top = bkgd->height, bot = 0, left = bkgd->width, right = 0;
    for(int i = 0; i < points.size(); ++i){
      top = min(top, (int)floor(points[i].y));
      bot = max(bot, (int)ceil(points[i].y));
      left = min(left, (int)floor(points[i].x));
      right = max(right, (int)ceil(points[i].x)); }
    image tmp(right-left+1, bot-top+1);
    vec<point> p;
    for(int i = 0; i < points.size(); ++i)
      p.pb(point(points[i].x-left, points[i].y-top));
    for(int i = 0; i < points.size()-1; ++i)
      line(p[i], p[i+1]).draw(&tmp);
    line(p.back(), p[0]).draw(&tmp);
    int x, y = (tmp.height >> 1);
    for(x = 0; x < tmp.width  ; ++x)
      if(tmp.data[y][x] == BLACK){ ++x; break; }
    queue<point> q;
    q.push(point(x, y));
    tmp.data[y][x] = BLACK;

    while(!q.empty()){
      point p = q.front();
      q.pop();
      x = (int)floor(p.x), y = (int)floor(p.y);
      if(tmp.data[y-1][x] == WHITE){
        q.push(point(p.x, p.y-1));
        tmp.data[y-1][x] = BLACK; }
      if(tmp.data[y+1][x] == WHITE){
        q.push(point(p.x, p.y+1));
        tmp.data[y+1][x] = BLACK; }
      if(tmp.data[y][x-1] == WHITE){
        q.push(point(p.x-1, p.y));
        tmp.data[y][x-1] = BLACK; }
      if(tmp.data[y][x+1] == WHITE){
        q.push(point(p.x+1, p.y));
        tmp.data[y][x+1] = BLACK; } }
    for(int i = 0; i < tmp.height; ++i)
      for(int j = 0; j < tmp.width; ++j)
        if(tmp.data[i][j] == BLACK && i+top >= 0 && i+top < bkgd->height
            && j+left >= 0 && j+left < bkgd->width)
          bkgd->data[i+top][j+left] = fill; } };

#endif

// POLYGON

#ifndef POLYGON_HH
#define POLYGON_HH

#include "line.hh"
#include "image.hh"

struct polygon : object {

  vec<point> points;

  polygon(){}
  polygon(const vec<point>& _p){ points = _p; }

  virtual void validate(){
    object::validate(); }

  point size() const {
    double xmin = INFD, xmax = -INFD, ymin = INFD, ymax = -INFD;
    for(int i = 0; i < points.size(); ++i)
      xmin = min(xmin, points[i].x),
      xmax = max(xmax, points[i].x),
      ymin = min(ymin, points[i].y),
      ymax = max(ymax, points[i].y);
    return point(xmax - xmin, ymax - ymin); }

  bool inside(const point& p) const {
    line ray(p, point(p.x, p.y + size().y));
    int n = 0;
    for(int i = 0; i < points.size()-1; ++i)
      if(line(points[i], points[i+1]).intersects(ray)) ++n;
    if(line(points.back(), points[0]).intersects(ray)) ++n;
    for(int i = 0; i < points.size(); ++i)
      if(ray.on_seg(points[i])) --n;
    return n & 1; }

  bool intersects(const polygon& o){
    // True if any points lie inside the other
    for(int i = 0; i < points.size(); ++i)
      if(o.inside(points[i])) return true;
    for(int i = 0; i < o.points.size(); ++i)
      if(inside(o.points[i])) return true;
    // True if any edges intersect other edges
    vec<line> v1, v2;
    for(int i = 0; i < points.size()-1; ++i)
      v1.pb(line(points[i], points[i+1]));
    v1.pb(line(points.back(), points[0]));
    for(int i = 0; i < o.points.size()-1; ++i)
      v2.pb(line(o.points[i], o.points[i+1]));
    v2.pb(line(o.points.back(), o.points[0]));
    for(int i = 0; i < v1.size(); ++i)
      for(int j = 0; j < v2.size(); ++j)
        if(v1[i].intersects(v2[j])) return true;
    // Otherwise false
    return false; }

  double area() const {
    double p1 = 0.0, p2 = 0.0;
    for(int i = 0; i < points.size()-1; ++i)
      p1 += points[i].x * points[i+1].y,
      p2 += points[i].y * points[i+1].x;
    p1 += points.back().x * points[0].y;
    p2 += points.back().y * points[0].x;
    return fabs(p1 - p2) / 2.0; }

  virtual point update(double ms){ return point(0, 0); }

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

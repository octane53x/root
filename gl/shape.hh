// SHAPE

#ifndef SHAPE_HH
#define SHAPE_HH

#include "obj.hh"

void line::draw(image* bkgd){
  int xi = (a.x < b.x) ? 1 : -1;
  int yi = (a.y < b.y) ? 1 : -1;
  int w = ((int)floor(b.x - a.x) * xi);
  int h = ((int)floor(b.y - a.y) * yi);
  bool xlong = w > h;
  double di = xlong ? ((double)h / w) : ((double)w / h);
  int j = (int)floor(xlong ? a.y : a.x);
  double d = j;
  for(int i = (int)floor(xlong ? a.x : a.y);
      i != (int)floor(xlong ? b.x : b.y);){
    int ii = i, jj = j, s;
    if(xlong) s = ii, ii = jj, jj = s;
    //! thickness: for(int t =
    bkgd->data[ii][jj] = fill_color;
    double dp = d;
    d += di;
    if((int)floor(d) != (int)floor(dp)) j += (xlong ? yi : xi);
    i += (xlong ? xi : yi); }
  bkgd->data[(int)floor(b.y)][(int)floor(b.x)] = fill_color; }

void polygon::draw(image* bkgd){
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
        bkgd->data[i+top][j+left] = fill_color; }

//! void circle::draw(image* bkgd){}

#endif

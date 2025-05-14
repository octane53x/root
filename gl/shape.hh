// SHAPE

#ifndef SHAPE_HH
#define SHAPE_HH

#include "gl_obj.hh"

image triangle(point a, point b, point c, color col){
  int top,bot,left,right;
  top = min(min(a.y, b.y), c.y);
  bot = max(max(a.y, b.y), c.y);
  left = min(min(a.x, b.x), c.x);
  right = max(max(a.x, b.x), c.x);
  image r(point(right-left+1, bot-top+1));

}

image rectangle(point topleft, point size, color col){
  image r(size);
  for(int i = 0; i < size.y; ++i)
    for(int j = 0; j < size.x; ++j)
      r.data[i][j] = col;
  return r; }

image circle(point center, int rad, color col){

}

#endif

// SHAPE

#ifndef SHAPE_HH
#define SHAPE_HH

#include "image.hh"

void line::draw(image* img){
  int xi = (a.x < b.x) ? 1 : -1;
  int yi = (a.y < b.y) ? 1 : -1;
  int w = ((b.x - a.x) * xi);
  int h = ((b.y - a.y) * yi);
  bool xlong = w > h;
  double di = xlong ? ((double)h / w) : ((double)w / h);
  int j = xlong ? a.y : a.x;
  double d = j;
  for(int i = xlong ? a.x : a.y; i <= (xlong ? b.x : b.y);){
    int ii = i, jj = j, s;
    if(xlong) s = ii, ii = jj, jj = s;
    //! for(int t =
    img->data[ii][jj] = col;
    double dp = d;
    d += di;
    if((int)floor(d) > (int)floor(dp)) j += xlong ? yi : xi;
    i += xlong ? xi : yi; } }

// image triangle(point a, point b, point c, color col){
//   int top,bot,left,right;
//   top = min(min(a.y, b.y), c.y);
//   bot = max(max(a.y, b.y), c.y);
//   left = min(min(a.x, b.x), c.x);
//   right = max(max(a.x, b.x), c.x);
//   image r(point(right-left+1, bot-top+1));

// }

// image rectangle(point topleft, point size, color col){
//   image r(size);
//   for(int i = 0; i < size.y; ++i)
//     for(int j = 0; j < size.x; ++j)
//       r.data[i][j] = col;
//   return r; }

// image circle(point center, int rad, color col){

// }

#endif

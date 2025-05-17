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
  int j = (xlong ? a.y : a.x);
  double d = j;
  for(int i = (xlong ? a.x : a.y); i != (xlong ? b.x : b.y);){
    int ii = i, jj = j, s;
    if(xlong) s = ii, ii = jj, jj = s;
    //! for(int t =
    img->data[ii][jj] = fill_color;
    double dp = d;
    d += di;
    if((int)floor(d) != (int)floor(dp)) j += (xlong ? yi : xi);
    i += (xlong ? xi : yi); }
  img->data[b.y][b.x] = fill_color; }

void triangle::draw(image* img){
  image tmp(img->size);
  line(a, b).draw(&tmp);
  line(a, c).draw(&tmp);
  line(b, c).draw(&tmp);
  line(a, b).draw(img);
  line(a, c).draw(img);
  line(b, c).draw(img);
  for(int i = 0; i < tmp.size.y; ++i){
    bool fill = false;
    int jt;
    for(int j = 0; j < tmp.size.x; ++j){
      if(tmp.data[i][j] == BLACK){
        fill = !fill;
        if(fill) jt = j;
        else for(int k = jt; k <= j; ++k)
          img->data[i][k] = fill_color; } } } }

// image rectangle(point topleft, point size, color col){
//   image r(size);
//   for(int i = 0; i < size.y; ++i)
//     for(int j = 0; j < size.x; ++j)
//       r.data[i][j] = col;
//   return r; }

// image circle(point center, int rad, color col){

// }

#endif

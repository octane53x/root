// 2D POINT

#ifndef POINT_HH
#define POINT_HH

#include "incl.hh"

struct point {

  // X and Y coordinates
  int x, y;

  point();
  point(const int _x, const int _y); };

point::point():
  x(0), y(0) {}

point::point(const int _x, const int _y):
  x(_x), y(_y) {}

#endif

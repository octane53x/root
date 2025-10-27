// MODEL

#ifndef MODEL_HH
#define MODEL_HH

#include "color.hh"
#include "point.hh"

struct triangle {

  point a,b,c;
  color fill;

  triangle(){}
  triangle(const point _a, const point _b, const point _c):
      a(_a), b(_b), c(_c) {} };

struct model {

  point box;
  vec<triangle> tris;

  model(){} };

#endif

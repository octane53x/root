// MODEL

#ifndef MODEL_HH
#define MODEL_HH

#include "image.hh"

struct triangle {

  point a,b,c;
  color fill;

  triangle(){}
  triangle(const point _a, const point _b, const point _c):
      a(_a), b(_b), c(_c) {} };

struct model : object {

  point hitbox;
  vec<triangle> triangles;

  model(){}

  virtual void validate(){
    object::validate(); }

  virtual point update(double ms){ return point(0, 0); }

  void draw(image* bkgd){
    //!
  } };

#endif

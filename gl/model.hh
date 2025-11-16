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

  void draw(image* bkgd){
    //! assume camera looking +z
  } };

#endif

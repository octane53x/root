// MODEL

#ifndef MODEL_HH
#define MODEL_HH

#include "image.hh"

struct model : object {

  struct tri {

    point a,b,c;
    color fill; //? textures

    tri(){}
    tri(const point _a, const point _b, const point _c):
        a(_a), b(_b), c(_c) {} };

  point hitbox;
  vec<tri> triangles;

  model(){}

  virtual void validate(){
    object::validate(); }

  void draw(image* bkgd){
    //! assume camera looking +z
  } };

#endif

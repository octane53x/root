// ABSTRACT BUTTON

#ifndef BUTTON_HH
#define BUTTON_HH

#include "polygon.hh"

// Polygon is the click hitbox
struct button : polygon {

  button(){}

  virtual void fn() = 0;

  bool click(point c){
    return inside(c); }

  virtual void validate(){
    polygon::validate(); }

  virtual point update(double ms){ return point(0, 0); }

  // object.draw passed through
};

#endif

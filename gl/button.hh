// ABSTRACT BUTTON

#ifndef BUTTON_HH
#define BUTTON_HH

#include "polygon.hh"

// Polygon is the click hitbox
struct button : polygon, image {

  button(){}

  virtual void validate(){
    polygon::validate();
    image::validate(); }

  virtual void fn() = 0;

  bool click(point c){
    if(inside(c)){
      fn();
      return true; }
    return false; } };

#endif

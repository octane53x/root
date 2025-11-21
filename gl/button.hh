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
    print(str("Clicked ")+to_string(c.x)+str(", ")+to_string(c.y)+str("\n"));
    if(inside(c)){
      fn();
      return true; }
    return false; }

  virtual point update(double ms){ return point(0, 0); }

  virtual void draw(image* bkgd){
    if(fill != CLEAR) polygon::draw(bkgd);
    else if(!image::empty()) image::draw(bkgd); } };

#endif

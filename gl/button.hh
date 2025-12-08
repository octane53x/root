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

  virtual void fn_env(env* e) = 0;
  virtual void fn_sys(system* s) = 0;

  bool click(point c){
    if(inside(c)){
      fn();
      return true; }
    return false; }

  virtual point update(double ms){ return point(0, 0); }

  virtual void draw(image* bkgd, viewport view){
    if(fill != CLEAR) polygon::draw(bkgd, view);
    else if(!image::empty()) image::draw(bkgd, view); } };

#endif

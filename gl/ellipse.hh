// ELLIPSE

#ifndef ELLIPSE_HH
#define ELLIPSE_HH

#include "image.hh"

struct ellipse : object {

  double rad1, rad2;
  point center;

  circle(){}
  circle(point c, int r){
      center = c, radius = r, fill_color = BLACK; }

  virtual void validate(){
    object::validate(); }

  virtual point update(double ms){ return point(0, 0); }

  void draw(image* bkgd){
    //!
  } };

#endif

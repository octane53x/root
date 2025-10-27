// ELLIPSE

#ifndef ELLIPSE_HH
#define ELLIPSE_HH

#include "point.hh"
#include "image.hh"

struct ellipse {

  double rad1, rad2;
  color fill;
  point center;

  circle(){}
  circle(point c, int r){
      center = c, radius = r, fill_color = BLACK; }

  void draw(image* bkgd){
    //!
  } };

#endif

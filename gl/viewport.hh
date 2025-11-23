// VIEWPORT

#ifndef VIEWPORT_HH
#define VIEWPORT_HH

#include "point.hh"

struct viewport : thing {

  double size;
  point topleft;

  viewport(): size(100.0), topleft(point(0, 0)) {}

  virtual void validate(){
    assert(size > 0.0, "viewport size not positive"); }

  point translate(point p, int frame_width, int frame_height){
    double ratio = min(frame_width, frame_height) / size;
    return p * ratio - topleft * ratio; } };

#endif

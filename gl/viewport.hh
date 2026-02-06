// VIEWPORT

#ifndef VIEWPORT_HH
#define VIEWPORT_HH

#include "point.hh"

// Window into the environment translating points to pixel coordinates
struct viewport {

  // Size in environment coordinates
  double size_in;
  // Size in output coordinates
  double size_out;
  // Position of the top left corner in the environment
  point top_left;

  viewport();

  virtual void validate(const str& func);

  point translate_in(const point& p) const;
  point translate_out(const point& p) const; };

// Set default member state
viewport::viewport(): size_in(100.0), size_out(100.0), top_left(point(0, 0)) {}

// Ensure valid state
void viewport::validate(const str& func){
  assert(size_in > 0.0 && size_out > 0.0, func, "viewport size not positive"); }

// Translate pixel coordinate to environment coordinate
point viewport::translate_in(const point& p) const {
  double ratio = size_in / size_out;
  return p * ratio + top_left; }

// Translate environment coordinate to pixel coordinate
point viewport::translate_out(const point& p) const {
  double ratio = size_out / size_in;
  return p * ratio - top_left * ratio; }

#endif

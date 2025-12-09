// VIEWPORT

#ifndef VIEWPORT_HH
#define VIEWPORT_HH

#include "point.hh"

// Window into the environment translating points to pixel coordinates
struct viewport : virtual thing {

  // Size in environment coordinates
  double size;
  // Position of the top left corner in the environment
  point topleft;

  viewport();

  virtual void validate(const str& func);

  point translate(const point& p,
      const int frame_width, const int frame_height) const; };

// Set default member state
viewport::viewport(): size(100.0), topleft(point(0, 0)) {}

// Ensure valid state
void viewport::validate(const str& func){
  assert(size > 0.0, func, "viewport size not positive"); }

// Translate environment coordinate to pixel coordinate
point viewport::translate(const point& p,
    const int frame_width, const int frame_height) const {
  double ratio = min(frame_width, frame_height) / size;
  return p * ratio - topleft * ratio; }

#endif

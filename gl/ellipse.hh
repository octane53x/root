// ELLIPSE

#ifndef ELLIPSE_HH
#define ELLIPSE_HH

#include "image.hh"

// A circular shape with a major and minor radius, can be a circle if equal
struct ellipse : object {

  // Major and minor radius
  double rad1, rad2;

  ellipse();
  ellipse(const double r1, const double r2);

  virtual void validate(const str& func) const;
  virtual void draw(image* canvas, const viewport& view); };

// Set default member state
ellipse::ellipse(): type("ellipse"), rad1(1.0), rad2(1.0) {}

// Construct with radii
ellipse::ellipse(const double r1, const double r2):
    type("ellipse"), rad1(r1), rad2(r2) {}

// Ensure valid state
void ellipse::validate(const str& func){
  object::validate(func);
  assert(dgt(rad1, 0.0) && dgt(rad2, 0.0), "ellipse radii not positive"); }

// Draw onto an image
void ellipse::draw(image* canvas, const viewport& view){
  //! draw
  validate("ellipse.draw"); }

#endif

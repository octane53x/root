// MODEL

#ifndef MODEL_HH
#define MODEL_HH

#include "image.hh"

// 3D collection of triangles
struct model : virtual object {

  // Triangle
  struct tri {
    // Triangle points
    point a,b,c;
    // Fill color
    color fill;

    tri(const point& _a, const point& _b, const point& _c); };

  // Hit box used to estimate ray collision without checking all triangles
  point hitbox_corner, hitbox_size;
  // The triangles in the model
  vec<tri> triangles;

  model();

  virtual void validate(const str& func);
  virtual void draw(image* canvas, const viewport& view); };

// Set default member state
model::model(){
  type = "model"; }

// Construct with three points
model::tri::tri(const point& _a, const point& _b, const point& _c):
    a(_a), b(_b), c(_c) {}

// Ensure valid state
void model::validate(const str& func){
  object::validate(func);
  assert(dgt(hitbox_size.x, 0.0) && dgt(hitbox_size.y, 0.0)
      && dgt(hitbox_size.z, 0.0), func, "model hitbox size not positive"); }

// Draw 3D model onto 2D image
void model::draw(image* canvas, const viewport& view){
  //! draw
}

#endif

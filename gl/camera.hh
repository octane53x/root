// CAMERA

#ifndef CAMERA_HH
#define CAMERA_HH

// Point and direction of view into a scene
struct camera : virtual object {

  // Unit vectors used as the angle of view
  uvec look, up;

  camera();

  virtual void validate(const str& func) const; };

// Set default member state
camera::camera(): look(0.0, 0.0, 1.0), up(0.0, 1.0, 0.0) {}

// Ensure valid state
void camera::validate(const str& func) const {
  object::validate(func);
  //! look/up 90 degrees apart
}

#endif

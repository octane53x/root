// CAMERA

#ifndef CAMERA_HH
#define CAMERA_HH

struct camera : object {

  uvec look, up;

  camera(): look(0.0, 0.0, 1.0), up(0.0, 1.0, 0.0) {}

  virtual void validate(){
    //! look/up 90 degrees apart
  }

  virtual point update(double ms){ return point(0, 0); }

  virtual void draw(image* bkgd, viewport view){}
};

#endif

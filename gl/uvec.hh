// UNIT VECTOR

#ifndef UVEC_HH
#define UVEC_HH

#include "../core/thing.hh"

struct uvec : thing {

  double x, y, z;

  uvec(): x(0.0), y(0.0), z(1.0) {}
  uvec(const double _x, const double _y, const double _z):
      x(_x), y(_y), z(_z) { validate(); }

  virtual void validate(){
    double len = sqrt(x * x + y * y + z * z);
    if(deq(len, 1.0)) return;
    x /= len, y /= len, z /= len;
    assert(deq(len, 1.0), "uvec.validate error"); }

  void rotate(const uvec& uv, const double deg){
    //!
  } };

#endif

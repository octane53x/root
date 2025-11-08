// UNIT VECTOR

#ifndef UVEC_HH
#define UVEC_HH

#include "../core/thing.hh"

struct uvec : thing {

  double xr, yr;

  uvec(): xr(0.0), yr(0.0) {}
  uvec(const double _xr, const double _yr): xr(_xr), yr(_yr) {}

  virtual void validate(){}

  void rotate(const uvec uv, const double deg){
    //!
  } };

#endif

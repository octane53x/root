// UNIT VECTOR

#ifndef UVEC_HH
#define UVEC_HH

#include "../../core/util.hh"

struct uvec {

  double xr, yr;

  uvec(): xr(0.0), yr(0.0) {}
  uvec(const double _xr, const double _yr): xr(_xr), yr(_yr) {}

  void rotate(const uvec uv, const double deg){
    //!
  } };

#endif

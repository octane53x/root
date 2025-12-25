// FUNCTION

#ifndef FN_HH
#define FN_HH

#include "../core/util.hh"

struct Fn {

  llu id;
  str name;
  vec<Fn*> code, params;

  void call(); };

void Fn::call(){}

#endif

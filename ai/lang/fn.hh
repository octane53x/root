// FUNCTION

#ifndef FN_HH
#define FN_HH

#include "thing.hh"

// Function with any number of parameters
struct Fn : virtual Thing {

  Fn(const Name& _name, Thing* _ctr);

  virtual void call(const vec<Var*>& params) = 0; };

// Construct with type, name, and container
Fn::Fn(const Name& _name, Thing* _ctr):
    Thing(_name, _ctr) {
  type = "Fn";
  reg.add(type, {"Thing"}); }

#endif

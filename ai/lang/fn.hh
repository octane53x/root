// FUNCTION

#ifndef FN_HH
#define FN_HH

#include "thing.hh"

// Function with any number of parameters
typedef struct Function : virtual Thing {

  Function();
  Function(const Type& _type, const str& _name, const Thing* _ctr);

  virtual void call(const vec<Var*>& params) = 0;

} Fn;

// Set default member state
Function::Function(): Thing() {
  type = "Fn";
  reg.add(type, {"Thing"}); }

// Construct with type, name, and container
Function::Function(const str& _name, const Thing* _ctr):
    Thing(_name, _ctr), Function() {}

#endif

// ABSTRACT VARIABLE

#ifndef VAR_HH
#define VAR_HH

#include "reg.hh"

// Abstract base class for primitives, structures, functions, and pointers
// Contains no data so derived primitive wrapper takes no extra memory
struct Var {

  Var();

  virtual str to_str() const = 0;
  virtual str get_state() const = 0;

  virtual void load_state(const str& state) = 0; };

// Set default member state
Var::Var(){
  reg.add("Var", {}); }

#endif

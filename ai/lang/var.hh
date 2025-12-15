// ABSTRACT VARIABLE

#ifndef VAR_HH
#define VAR_HH

#include "reg.hh"

// Abstract base class for both primitives and structures
// Contains almost no data so derived primitive wrapper takes no extra memory
typedef struct Variable {

  // Whether this is a primitive, for casting
  bool prim;

  Variable();

  virtual str to_str() const = 0;
  virtual str get_state() const = 0;

  virtual void load_state(const str& state) = 0;

} Var;

// Set default member state
Var::Var(): prim(true) {
  reg.add("Var", {}); }

#endif

// ABSTRACT THING

#ifndef THING_HH
#define THING_HH

#include "type.hh"

struct str;

// Base class for everything
struct thing : virtual type<thing> {

  // Whether validations are executed, turned on for debug
  inline static bool validate_on = false;

  // Convert to string
  virtual str to_str() const = 0;
  // Ensure valid state, making assertions and changes
  virtual void _validate(const str& fn) = 0;
  void validate(const str& fn); };

void thing::validate(const str& fn){
  if(validate_on){
    assert(type_id != 0, fn, "type not set");
    _validate(fn); } }

#endif

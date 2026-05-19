// ABSTRACT THING

#ifndef THING_HH
#define THING_HH

#include "def.hh" //!
//#include "type.hh"

struct Str;
enum class PrintMode;

// Base class for everything
struct Thing /*: virtual type<thing>*/ {

  // Whether validations are executed, turned on for debug
  inline static bool validate_on = false;

  // Convert to string
  virtual Str to_str(const PrintMode mode) const = 0;

  // Ensure valid state
  virtual void _validate(const Str& fn) = 0;
  void validate(const Str& fn); };

void Thing::validate(const Str& fn){
  if(!validate_on)
    return;
  //assert(type_id != 0, fn, "type not set");
  _validate(fn); }

#endif

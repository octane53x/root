// ABSTRACT THING

#ifndef THING_HH
#define THING_HH

#include "util.hh"

// Base class for everything
struct thing {

  // Ensure valid state
  virtual void validate(str func) = 0; };

#endif

// ABSTRACT THING

#ifndef THING_HH
#define THING_HH

#include "util.hh"

// Base class for everything
struct thing {

  virtual void validate(const str& func) = 0; // Ensure valid state
  virtual str to_str() const; };

// Convert to string
str thing::to_str() const {
  return str("thing"); }

#endif

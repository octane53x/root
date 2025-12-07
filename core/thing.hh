// ABSTRACT THING

#ifndef THING_HH
#define THING_HH

#include "util.hh"

// Base class for everything
struct thing {

  virtual ~thing() = 0; // Make class abstract

  virtual void validate(str func); };

// Implemented to avoid linker error
thing::~thing(){}

// Call at the end of every object function
// Report which function called the failed validate
void validate(str func){
  print(str("!! validate failed in ")+func+str(":\n")); }

#endif

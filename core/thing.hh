// ABSTRACT THING

#ifndef THING_HH
#define THING_HH

#include "util.hh"

struct thing {
  virtual void validate() = 0;
};

#endif

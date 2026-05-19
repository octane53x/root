// FLAG

#ifndef FLAG_HH
#define FLAG_HH

#include "thing.hh"

enum FLAG : uchar {FALSE, TRUE, NONE, UNKNOWN};

struct Flag : virtual Thing {

  // Storage
  FLAG data;

  // Constructors
  Flag();
  // Defined in ops/prim.hh
  Flag(const bool b);
};

// Set default state
Flag::Flag():
  data(NONE) {}

#endif

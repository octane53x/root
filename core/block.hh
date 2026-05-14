// 64-BIT MEMORY UNIT

#ifndef BLOCK_HH
#define BLOCK_HH

#include "incl.hh"

struct block : virtual thing {

  // Primitive data block
  unsigned long long data;

  // Prints the hex value
  // Defined in conv.hh
  struct str;
  virtual str to_str();

  // Sets to zero
  void reset();
};

void block::reset(){
  data = 0; }

#endif

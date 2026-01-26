// INSTRUCTION

#ifndef INSTR_HH
#define INSTR_HH

#include "util.hh"

struct Fn;

//*** RECURSIVE ***
// Nested instructions as specific function parameters
struct Instr {

  // Primary operation
  Fn* fn;
  // Nested function calls that resolve to Vars to supply as parameters
  vec<Instr> params;

  void exec(); };

// Recursively calls functions to set function param values
void Instr::exec(){}

#endif

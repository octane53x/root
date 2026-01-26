// FUNCTION

#ifndef FN_HH
#define FN_HH

#include "var.hh"
#include "instr.hh"

// Sequence of instructions with unset parameters
struct Fn {

  // Identifier
  str name;
  // Var without address
  vec<Var> params;
  // Sequence of instructions to execute
  vec<Instr> code;

  void call(); };

// Iterate through instructions, executing each
void Fn::call(){
  for(const Instr& instr : code)
    instr.exec(); }

#endif

// FUNCTION

#ifndef FN_HH
#define FN_HH

#include "type.hh"
#include "instr.hh"

// Sequence of instructions with unset parameters
struct Fn {

  // False if only forward declared
  bool defined;
  // Identifier
  str name;
  // Containing type
  Type* ctr;
  // Modifiers
  uset<str> mods;
  // Var without address
  vec<Var> params;
  // Sequence of instructions to execute
  vec<Instr> code;
  // Possible return types
  uset<Type*> returns;

  Fn();

  void call(); };

// Set default member state
Fn::Fn(): defined(false), ctr(NULL) {}

// Iterate through instructions, executing each
void Fn::call(){
  for(Instr& instr : code)
    instr.exec(); }

#endif

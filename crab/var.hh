// VARIABLE

#ifndef VAR_HH
#define VAR_HH

#include "type.hh"
#include "allocator.hh"

struct Var {

  Type type;
  void* addr;
  static Allocator* allocator;

  Var(); };

Var::Var(): addr(NULL) {}

#endif

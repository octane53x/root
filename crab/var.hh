// VARIABLE

#ifndef VAR_HH
#define VAR_HH

#include "type.hh"
#include "allocator.hh"

const int
    SIZE_BOOL = 8;

struct Var {

  Type type;
  void* addr;
  static Allocator* allocator;

  Var();
  Var(const bool b); };

Var::Var(): addr(NULL) {}

Var::Var(const bool b): type("bool"), addr(allocator->allocate(SIZE_BOOL)) {}

#endif

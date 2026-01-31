// VARIABLE

#ifndef VAR_HH
#define VAR_HH

#include "util.hh"

struct Type;

struct Var {

  // Identifier
  str name;
  // Structure
  Type* type;
  // Address
  void* addr;
  // Modifiers
  uset<str> mods;

  Var(); };

Var::Var(): type(NULL), addr(NULL) {}

#endif

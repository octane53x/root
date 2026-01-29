// TYPE

#ifndef TYPE_HH
#define TYPE_HH

#include "var.hh"

struct Fn;

struct Type {

  // False if only forward declared
  bool defined;
  // Identifier
  str name;
  // Containing type
  Type* ctr;
  // Modifiers
  uset<str> mods;
  // Direct parents
  uset<Type*> bases;
  // Member variables without address
  umap<str, Var> vars;
  // Member functions
  uset<Fn*> fns;

  Type(); };

// Set default member state
Type::Type(): defined(false) {}

#endif

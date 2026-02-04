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
  umap<str, Type*> bases;
  // Internal types
  umap<str, Type> types;
  // Member variables without address
  umap<str, Var> vars;
  // Member functions
  umap<str, Fn> fns;

  Type();

  Type* get_type(const str& type) const; };

// Set default member state
Type::Type(): defined(false), ctr(NULL) {}

Type* Type::get_type(const str& type) const {
  if(contains(types, type)) return types[type];
  if(ctr == NULL) return NULL;
  return ctr->get_type(type); }

#endif

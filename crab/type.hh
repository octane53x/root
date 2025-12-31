// TYPE

#ifndef TYPE_HH
#define TYPE_HH

#include "typemgr.hh"

struct Type {

  _Type* data;
  static TypeMgr* registry;

  Type();
  Type(const str& s);

  Type& operator=(const str& s);

  size_t size() const; };

Type::Type(): Type("Void") {}

Type::Type(const str& s){
  *this = s; }

Type& Type::operator=(const str& s){
  name = &registry->types[s].name;
  return *this; }

size_t Type::size() const {
  return registry->types[*name].size; }

#endif

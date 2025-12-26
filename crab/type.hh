// TYPE

#ifndef TYPE_HH
#define TYPE_HH

#include "typemgr.hh"

struct Type {

  str* name;
  static TypeMgr* registry;

  Type();
  Type(const str& s);

  Type& operator=(const str& s); };

Type::Type(): Type("void") {}

Type::Type(const str& s){
  *this = s; }

Type& Type::operator=(const str& s){
  registry->add(s, vec<str>());
  name = &registry->types[s].name;
  return *this; }

#endif

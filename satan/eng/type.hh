// TYPE

#ifndef TYPE_HH
#define TYPE_HH

//#include "dimension.hh"
#include "thing.hh" //!

struct Type /*: virtual Dimension*/ {

  str name;
  static umap<str, Type> registry;

  Type();
  Type(const str& _name);

  bool operator==(const Type& o) const;

  static Type* get(const str& _name); };

umap<str, Type> Type::registry;

Type::Type(): name("Void") {}
Type::Type(const str& _name): name(_name) {}

bool Type::operator==(const Type& o) const {
  return this == &o; }

Type* Type::get(const str& _name){
  assert(registry.find(_name) != registry.end(),
      "Type.get", "Type not registered");
  return &registry[_name]; }

Thing::Thing(){
  type = Type::get("Thing"); }

Thing::register_type(){


#endif

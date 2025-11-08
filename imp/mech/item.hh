// ITEM MECHANIC

#ifndef ITEM_HH
#define ITEM_HH

#include "../../gl/image.hh"

struct Item : thing {

  str name;
  image* icon;

  Item(){}

  virtual void validate(){} };

namespace std {
  template <>
  struct hash<Item> {
    size_t operator()(const Item& item) const {
      return hash<str>()(item.name); } }; }

#endif

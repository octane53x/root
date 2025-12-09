// ITEM MECHANIC

#ifndef ITEM_HH
#define ITEM_HH

#include "../../gl/image.hh"

struct Item : virtual thing {

  int count;
  str name;
  image* icon;

  Item(){}

  virtual void validate(const str& func){} };

#endif

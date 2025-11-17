// ITEM MECHANIC

#ifndef ITEM_HH
#define ITEM_HH

#include "../../gl/image.hh"

struct Item : thing {

  int count;
  str name;
  image* icon;

  Item(){}

  virtual void validate(){} };

#endif

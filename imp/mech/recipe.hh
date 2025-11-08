// RECIPE MECHANIC

#ifndef RECIPE_HH
#define RECIPE_HH

#include "item.hh"

struct Recipe : thing {

  map<Item, int> parts, product;

  Recipe(){}

  virtual void validate(){} };

#endif

// RECIPE

#ifndef RECIPE_HH
#define RECIPE_HH

#include "../gl/gl_obj.hh"

struct Recipe : thing {
  map<str, int> parts, product;
  Recipe(){ type = "Recipe"; } };

#endif

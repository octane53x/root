// MINERAL NODE

// Nodes generated in earth.hh

#ifndef NODE_HH
#define NODE_HH

#include "../mech/item.hh"

// + SOIL, SAND, STONE
const vec<str> MINERALS = {
  "COAL",
  "IRON",
  "COPPER",
  "ALUMINUM",
  "LIMESTONE",
  "GRANITE",
  "MARBLE",
  "LEAD",
  "TIN",
  "ZINC",
  "NICKEL",
  "GYPSUM",
  "LITHIUM",
  "SULFUR",
  "PHOSPHORUS",
  "SILICON",
  "TITANIUM",
  "SILVER",
  "GOLD",
  "PLATINUM",
  "ADAMANTIUM",
  "URANIUM",
  "NEPTUNIUM",
  "PLUTONIUM" };

struct Node : object {

  Item mineral;
  uset<point> form;

  Node(){}

  virtual void validate(){}

  virtual point update(double ms){ return point(0, 0); }

  virtual void draw(image* bkgd){} };

#endif

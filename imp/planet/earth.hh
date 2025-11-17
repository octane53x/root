// EARTH GENERATOR

#ifndef EARTH_HH
#define EARTH_HH

#include "block.hh"
#include "node.hh"

struct Earth : thing {

  vec<Node> nodes;
  vec<Block> mods;

  virtual void validate(){}

  void gen_planet(int planet_size){
    //!
  } };

#endif

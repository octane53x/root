// EARTH GENERATOR

#ifndef EARTH_HH
#define EARTH_HH

#include "block.hh"
#include "node.hh"

struct Earth : virtual system {

  vec<Node> nodes;
  vec<Block> mods;

  Earth(){ type = "Earth"; }

  void gen_planet(int planet_size){
    //!
  } };

#endif

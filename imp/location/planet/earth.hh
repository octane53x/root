// EARTH GENERATOR

#ifndef EARTH_HH
#define EARTH_HH

#include "block.hh"
#include "node.hh"

// Underground of a planet
struct Earth : virtual system {

  // Mineral nodes
  vec<Node> nodes;

  Earth();

  void gen_planet(const int planet_size); };

// Set default member state
Earth::Earth(){
  type = "Earth"; }

// Create mineral nodes
void Earth::gen_planet(const int planet_size){}

#endif

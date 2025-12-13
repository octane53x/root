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

  Block* get_block(const point& p);
  void gen_planet(const int planet_size); };

// Set default member state
Earth::Earth(){
  type = "Earth"; }

// Get the block at supplied coordinate
Block* Earth::get_block(const point& p){
  Block* b = new Block();
  b->pos = point(floor(p.x), floor(p.y), floor(p.z));
  //! Temporary. Fill block with random minerals
  for(int i = 0; i < 100; ++i){
    str m = MINERALS[rand() % MINERALS.size()];
    if(b->minerals.find(m) == b->minerals.end()){
      Item item;
      item.name = m;
      item.count = 1;
      b->minerals[m] = item;
    }else
      ++b->minerals[m].count; }
  return b; }

// Create mineral nodes
void Earth::gen_planet(const int planet_size){}

#endif

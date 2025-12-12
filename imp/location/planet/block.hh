// UNDERGROUND BLOCK

#ifndef BLOCK_HH
#define BLOCK_HH

#include "../../../gl/object.hh"

// Underground 1x1x1 block of minerals
struct Block : virtual object {

  // Whether a tunnel has been dug through the block
  bool tunnel;
  // Progress 0.0-1.0 on mining the next mineral or digging a tunnel
  double prog_mine, prog_tunnel;
  // Remaining minerals from overlapping nodes
  map<str, int> minerals;
  // Adjacent blocks, attached if this has a tunnel
  vec<Block*> adj;

  Block();

  virtual void draw(image* canvas, const viewport& view);

  bool in_block(const point& p); };

// Set default member state
Block::Block(): tunnel(false), prog_mine(0.0), prog_tunnel(0.0) {
  type = "Block"; }

// Implemented to remove abstraction
void Block::draw(image* canvas, const viewport& view){}

// Is a coordinate in this block
bool Block::in_block(const point& p){
  return dgeq(p.x, pos.x) && dlt(p.x, pos.x + 1.0)
      && dgeq(p.y, pos.y) && dlt(p.y, pos.y + 1.0)
      && dgeq(p.z, pos.z) && dlt(p.z, pos.z + 1.0); }

#endif

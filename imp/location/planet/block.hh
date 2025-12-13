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
  map<str, Item> minerals;
  // Adjacent blocks, attached if this has a tunnel
  vec<Block*> adj;

  Block();

  virtual void draw(image* canvas, const viewport& view);

  bool in_block(const point& p) const;
  int mineral_count() const;

  Item mine(); };

// Set default member state
Block::Block(): tunnel(false), prog_mine(0.0), prog_tunnel(0.0) {
  type = "Block"; }

// Implemented to remove abstraction
void Block::draw(image* canvas, const viewport& view){}

// Is a coordinate in this block
bool Block::in_block(const point& p) const {
  return dgeq(p.x, pos.x) && dlt(p.x, pos.x + 1.0)
      && dgeq(p.y, pos.y) && dlt(p.y, pos.y + 1.0)
      && dgeq(p.z, pos.z) && dlt(p.z, pos.z + 1.0); }

// How many minerals remain
int Block::mineral_count() const {
  int r = 0;
  for(pair<str, Item> p : minerals)
    r += p.second.count;
  return r; }

// Mine the block, returns a random mineral
Item Block::mine(){
  vec<Item> vi;
  for(pair<str, Item> p : minerals)
    vi.pb(p.second);
  vec<int> vs;
  vs.pb(vi[0].count);
  for(int i = 1; i < vi.size(); ++i)
    vs.pb(vi[i].count + vs[i-1]);
  int r = rand() % vs.back() + 1;
  Item item;
  for(int i = 0; i < vs.size(); ++i)
    if(r <= vs[i]){
      item = vi[i];
      item.count = 1;
      --minerals[item.name].count;
      if(minerals[item.name].count == 0)
        minerals.erase(minerals.find(item.name));
      break; }
  assert(item.count == 1, "block.mine", "mine not returning one mineral");
  return item; }

#endif

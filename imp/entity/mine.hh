// MINE ENTITY

#ifndef MINE_HH
#define MINE_HH

#include "../../core/graph.hh"
#include "../location/planet/planet.hh"

// A mine to harvest minerals found on a planet
struct Mine : virtual Entity {

  // Entry block to the tunnels beneath
  Block* top;
  // Blocks currently assigned to be tunnneled
  uset<Block*> tunnel_assn;
  // Priority blocks, removed when worker is assigned
  map<int, Block*> prio;
  // Graph of all blocks for finding paths
  graph<Block*> blocks;

  Mine(Location* _loc);

  virtual void init();
  virtual void draw(image* canvas, const viewport& view);
  virtual void update_game();

  Block* next_tunnel(); };

// Set default member state
Mine::Mine(Location* _loc): Entity(_loc) {
  type = "Mine";
  top = NULL;
  hitbox_size = point(2.0, 2.0, 1.0); }

// Set the top block
void Mine::init(){
  Planet* planet = dynamic_cast<Planet*>(loc);
  top = planet->earth.get_block(point(pos.x, pos.y, pos.z + 1.0)); }

// Draw onto image
void Mine::draw(image* canvas, const viewport& view){
  polygon poly;
  point p(pos.x - 1.0, pos.y - 1.0);
  poly.add(p);
  poly.add(point(p.x + 2.0, p.y));
  poly.add(point(p.x + 2.0, p.y + 2.0));
  poly.add(point(p.x, p.y + 2.0));
  poly.fill = PURPLE;
  poly.draw(canvas, view);
  validate("Mine.draw"); }

// Mine operations ???
void Mine::update_game(){}

// Find a block to assign a tunneling operation
// Returns NULL if no available blocks
Block* Mine::next_tunnel(){
  queue<Block*> q;
  uset<Block*> vis;
  q.push(top);
  while(!q.empty()){
    Block* b = q.front();
    q.pop();
    if(b->tunnel == false && !contains(tunnel_assn, b)){
      tunnel_assn.insert(b);
      return b; }
    vis.insert(b);
    for(Block* t : b->adj)
      if(!contains(vis, t))
        q.push(t); }
  return NULL; }

#endif

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
  // Set of all blocks, queried by position
  umap<point, Block*> block_by_pt;
  // Graph of all tunneled blocks for finding paths
  graph<Block*> blocks;

  // Minerals brought to top, not yet shipped
  umap<str, Item> stockpile;

  Mine(Location* _loc);

  virtual void init();
  virtual void draw(image* canvas, const viewport& view);
  virtual void update_game(const double tick);

  int block_score(Block* block) const;

  Block* next_tunnel();
  queue<point> path_convert(const vec<Block*>& block_path);
  void add_to_stockpile(const umap<str, Item>& inventory); };

// Set default member state
Mine::Mine(Location* _loc): Entity(_loc) {
  type = "Mine";
  top = NULL;
  hitbox_size = point(2.0, 2.0, 1.0); }

// Set the top block
void Mine::init(){
  Planet* planet = dynamic_cast<Planet*>(loc);
  top = planet->earth.get_block(point(pos.x, pos.y, pos.z + 1.0));
  prio[block_score(top)] = top;
  block_by_pt[top->pos] = top; }

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
void Mine::update_game(const double tick){}

// Find a priority score for a block based on its minerals
//! Query the city market
int Mine::block_score(Block* block) const {
  return (block->mineral_count() > 0) ? 1 : 2; }

// Find a block without a tunnel and assign as a tunneling operation
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

// Convert the result from graph.path to a Unit path
queue<point> Mine::path_convert(const vec<Block*>& block_path){
  queue<point> r;
  for(Block* b : block_path)
    r.push(point(b->pos.x + 0.5, b->pos.y + 0.5, b->pos.z + 0.5));
  return r; }

// Add unit inventory to mine stockpile
void Mine::add_to_stockpile(const umap<str, Item>& inventory){
  umap<str, Item>::iterator it;
  for(pair<str, Item> p : inventory){
    if((it = stockpile.find(p.second.name)) == stockpile.end())
      stockpile[p.second.name] = p.second;
    else
      it->second.count += p.second.count; } }

#endif

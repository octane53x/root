// WORKER UNIT

#ifndef WORKER_HH
#define WORKER_HH

#include "../mine.hh"

const double
    // How far the center of a block can be to mine it
    BLOCK_RANGE = 1.2,
    // How long it takes to mine one mineral from a block
    MINE_TIME = 5.0,
    // How long it takes to dig a tunnel
    DIG_TIME = 5.0; //! short for testing

// Worker unit for building, mining, farming
struct Worker : Unit {

  // What the worker is currently doing
  enum Activity {
    // Doing nothing
    IDLE,
    // Moving to a block or surface destination
    TRAVEL,
    // Returning to offload site to dump inventory contents
    OFFLOAD,
    // Mining a block
    MINE,
    // Digging a tunnel
    DIG,
    // Planting seeds on a farm
    PLANT,
    // Harvesting crops from a farm
    HARVEST,
    // Constructing a building
    BUILD
  } activity;

  // Worker is inside the tunnel of this block
  Block* block_loc;
  // Block being mined
  Block* target;
  // Mine the worker is working
  Mine* mine;

  Worker();

  virtual void validate(const str& func);
  virtual void update(const double ms);
  virtual void update_game(const double tick);

  bool block_in_range(const Block* b); };

// Set default member state
Worker::Worker(): activity(IDLE), block_loc(NULL), target(NULL), mine(NULL) {
  type = "Worker"; }

// Ensure valid state
void Worker::validate(const str& func){
  Unit::validate(func);
  assert(block_loc != NULL, func, "block_loc is NULL");
  assert(mine != NULL, func, "mine is NULL"); }

// Worker movement
void Worker::update(const double ms){
  str func("Worker.update");
  validate(func);
  Unit::update(ms);
  if(!block_loc->in_block(pos)){
    point p(floor(pos.x), floor(pos.y), floor(pos.z));
    assert(contains(mine->block_by_pt, p), func, "pos not in block_by_pt");
    block_loc = mine->block_by_pt(p); }
  validate(func); }

// Worker operations
void Worker::update_game(const double tick){
  str func("Worker.update_game");
  validate(func);

  // Select an activity
  if(activity == IDLE){
    assert(path.empty(), func, "IDLE but path not empty");

    // Get path to top, switch to OFFLOAD
    if(inventory_size() == inventory_cap){
      assert(target == NULL, func, "Ready to OFFLOAD but target is set");
      path = mine->path_convert(mine->blocks.path(block_loc, mine->top));
      activity = OFFLOAD;

    // Get target and path, switch to TRAVEL
    }else if(target == NULL){
      if(mine->prio.empty())
        target = mine->next_tunnel();
      else{
        target = mine->prio.begin()->second;
        mine->prio.erase(mine->prio.begin()); }
      path = mine->path_convert(mine->blocks.path(block_loc, target));
      activity = TRAVEL;

    // If in range, switch to MINE or DIG
    }else if(block_in_range(target)){
      if(contains(mine->tunnel_assn, target))
        activity = DIG;
      else
        activity = MINE; } }

  // Operations
  switch(activity){
  case IDLE:
    err(func, "failed to set activity to non-IDLE");

  // If arrived, go to IDLE
  case TRAVEL:
    if(path.empty())
      activity = IDLE;
    break;

  // If arrived, dump inventory, go to IDLE
  case OFFLOAD:
    if(path.empty()){
      mine->add_to_stockpile(inventory);
      inventory.clear();
      activity = IDLE; }
    break;

  // Advance progress on mining the target block
  case MINE:
    target->prog_mine += tick;
    if(dgeq(target->prog_mine, 1.0)){
      add_to_inventory(target->mine());
      target->prog_mine = 0.0;
      if(inventory_size() == inventory_cap){
        mine->prio[mine->block_score(target)] = target;
        target = NULL;
        activity = IDLE;
      }else if(target->mineral_count() == 0){
        target = NULL;
        activity = IDLE; } }
    break;

  // Advance progress on digging the tunnel
  case DIG:
    target->prog_tunnel += tick;
    if(dgeq(target->prog_tunnel, 1.0)){
      target->tunnel = true;
      mine->tunnel_assn.erase(mine->tunnel_assn.find(target));
      // Add tunneled block to graph

      // Attach new blocks
      vec<point> adj_pos;
      point p = target->pos;
      adj_pos.pb(point(p.x - 1.0, p.y, p.z));
      adj_pos.pb(point(p.x + 1.0, p.y, p.z));
      adj_pos.pb(point(p.x, p.y - 1.0, p.z));
      adj_pos.pb(point(p.x, p.y + 1.0, p.z));
      adj_pos.pb(point(p.x, p.y, p.z - 1.0));
      adj_pos.pb(point(p.x, p.y, p.z + 1.0));
      Planet* planet = dynamic_cast<Planet*>(mine->loc);
      for(const point& adj : adj_pos)
        if(mine->block_by_pt.find(adj) == mine->block_by_pt.end()){
          Block* b = planet->earth.get_block(adj);
          mine->prio[mine->block_score(b)] = b;
          mine->block_by_pt[b->pos] = b; }
      target = NULL;
      activity = IDLE;
    }
    break;

  default:
    err(func, "unhandled activity"); }
  validate(func); }

// Whether a block can be mined at the worker's current position
bool Worker::block_in_range(const Block* b){
  point p(b->pos.x + 0.5, b->pos.y + 0.5, b->pos.z + 0.5);
  return dleq(line(pos, p).len(), BLOCK_RANGE); }

#endif

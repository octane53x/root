// WORKER UNIT

#ifndef WORKER_HH
#define WORKER_HH

#include "../mine.hh"

const double
    // How far the center of the block can be to mine it
    BLOCK_RANGE = 1.2;

// Worker unit for building, mining, farming
struct Worker : Unit {

  // What the worker is currently doing
  enum Activity {
    // Doing nothing
    IDLE,
    // Mining a block
    MINE,
    // Digging a tunnel
    DIG,
    // Moving to a block or surface destination
    TRAVEL,
    // Returning to offload site to dump inventory contents
    OFFLOAD,
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
  // Path to a block target issued by the mine
  queue<Block*> path;
  // Mine the worker is working
  Mine* mine;

  Worker();

  virtual void update(const double ms);
  virtual void update_game();

  bool block_in_range(const Block* b); };

// Set default member state
Worker::Worker(): activity(IDLE), block_loc(NULL), target(NULL), mine(NULL) {
  type = "Worker"; }

// Worker movement
void Worker::update(const double ms){
  str func("Worker.update");
  assert(block_loc != NULL, func, "block_loc is NULL");
  assert(mine != NULL, func, "mine is NULL");

  Unit::update(ms);
  if(!block_loc->in_block(pos)){
    bool found = false;
    for(Block* adj : block_loc->adj)
      if(adj->in_block(pos)){
        block_loc = adj;
        found = true;
        break; }
    if(!found)
      err(func, "worker moved too far"); } }

// Worker operations
void Worker::update_game(){
  str func("Worker.update_game");
  assert(block_loc != NULL, func, "block_loc is NULL");
  assert(mine != NULL, func, "mine is NULL");

  switch(activity){
  // Begin an activity
  case IDLE:
    assert(target == NULL, func, "activity is IDLE but target is set");
    assert(path.empty(), func, "activity is IDLE but path is set");
    if(inventory_size() == inventory_cap){
      target = NULL;
      path = mine->blocks.path(block_loc, mine->top);
      activity = OFFLOAD;
    }else if(block_in_range(target){
      if(contains(mine->tunnel_assn, target))
        activity = DIG;
      else
        activity = MINE;
    } //!
    return;

  // Mine the block target
  case MINE:
    assert(target != NULL, func, "activity is MINE but target is NULL");
    assert(block_in_range(target), func,
        "activity is MINE but target block not in range");
    //!
    return;

  // Dig a tunnel
  case DIG:
    assert(target != NULL, func, "activity is DIG but target is NULL");
    assert(block_in_range(target), func,
        "activity is DIG but target block not in range");
    //!
    return;

  // Travel to block target
  case TRAVEL:
    assert(target != NULL, func, "activity is TRAVEL but target is NULL");
    assert(!path.empty(), func, "activity is TRAVEL but path is empty");
    //!
    return;

  // Return to top of mine
  case OFFLOAD:
    assert(target == NULL, func, "activity is OFFLOAD but target is set");
    assert(!path.empty(), func, "activity is OFFLOAD but path is empty");
    //!
    return;

  // Otherwise error
  default:
    err(func, "unhandled activity"); } }

// Whether a block can be mined at the worker's current position
bool Worker::block_in_range(const Block* b){
  point p(b->pos.x + 0.5, b->pos.y + 0.5, b->pos.z + 0.5);
  return dleq(line(pos, p).len(), BLOCK_RANGE); }

#endif

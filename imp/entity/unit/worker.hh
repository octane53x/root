// WORKER UNIT

#ifndef WORKER_HH
#define WORKER_HH

#include "../planet/planet.hh"

struct Worker : Unit {

  enum Activity {
    IDLE,
    DIG,
    TRAVEL,
    OFFLOAD,
    MINE,
    BUILD };

  Activity activity;
  int path_index;
  Block* target;
  vec<Block*> path;

  Worker(){ type = WORKER; }

  virtual void validate(){
    Unit::validate(); }

  int inventory_size(){
    int r = 0;
    map<str, int>::iterator it;
    for(it = inventory.begin(); it != inventory.end(); ++it)
      r += it->second;
    return r; }

  //! Add node to mine.nodes when discovered or offloading
  virtual void game_update(Planet* planet){
    // Block* block = &planet->earth[(int)loc.x][(int)loc.y][(int)loc.z];

    // if(activity == Activity::IDLE) pass();

    // else if(activity == Activity::DIG){ //! not digging current block
    //   assert(target != NULL, func, "Worker target not set");
    //   target->progress += GAME_TICK / DIG_TIME;
    //   if(target->progress >= 100.0){
    //     target->tunnel = true;
    //     activity = Activity::IDLE; } }

    // else if(activity == Activity::TRAVEL){
    //   // Decide to dig or mine

    //   if(path.empty()){

    //   }
    // }

    // //! Return to top of mine
    // else if(activity == Activity::OFFLOAD) pass();

    // else if(activity == Activity::MINE){
    //   assert(target != NULL, func, "Worker target not set");
    //   target->progress +=
    //       GAME_TICK / (BASE_MINING_TIME + MINING_SCALE * block->loc.z);
    //   if(target->progress >= 100.0){

    //     // Choose random mineral from block to mine
    //     int n = 0, r = rand() % block->total();
    //     map<str, int>::iterator it;
    //     for(it = block->minerals.begin(); it != block->minerals.end(); ++it){
    //       n += it->second;
    //       if(n > r){
    //         m = it->first;
    //         break; } }

    //     // Add mineral to worker inventory
    //     map<str, int>::iterator it2;
    //     if((it2 = inventory.find(it->first)) == inventory.end())
    //       inventory[it->first] = 1;
    //     else ++it2;

    //     // Remove mineral from block
    //     --it->second;
    //     if(it->second == 0) block->minerals.erase(it);

    //     // Resume mining unless inventory full or block empty
    //     target->progress = 0.0;
    //     if(inventory_size() == WORKER_LOAD_SIZE) activity = Activity::OFFLOAD;
    //     else activity = Activity::IDLE; } }
  }

  virtual point update(double ms){ return point(0, 0); } };

#endif

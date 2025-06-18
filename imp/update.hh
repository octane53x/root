// UPDATE METHODS

#ifndef UPDATE_HH
#define UPDATE_HH

#include "planet.hh"

void Mine::update(Planet* planet){
  for(Worker* worker : workers){
    if(worker->activity == Activity::IDLE){
      worker->target = nodes.front();
      nodes.pop();
      graph<Block*> g;
      queue<Block*> q;
      uset<Block*> v; //? remove?
      point p = worker->loc;
      Block* b0 = &planet->earth[(int)p.x][(int)p.y][(int)p.z];
      g.insert(b0);
      q.push(b0);
      v.insert(b0);

      // Construct mine graph
      while(!q.empty()){
        Block* bt = q.front();
        q.pop();
        for(Block* b : bt->adjacent){
          if(!b->tunnel || v.find(b) != v.end()) continue;
          g.insert(b);
          g.link(bt, b, 1);
          q.push(b);
          v.insert(b); } }

      // Shortest path to target


      worker->activity = Activity::TRAVEL; } } }

//! Add node to mine.nodes when discovered or offloading
void Worker::update(Planet* planet){
  Block* block = &planet->earth[(int)loc.x][(int)loc.y][(int)loc.z];

  if(activity == Activity::IDLE) pass();

  else if(activity == Activity::DIG){ //! not digging current block
    assert(target != NULL, "Worker target not set");
    target->progress += GAME_TICK / DIG_TIME;
    if(target->progress >= 100.0){
      target->tunnel = true;
      activity = Activity::SEARCH; } }

  else if(activity == Activity::TRAVEL){
    // Decide to dig or mine

    if(path.empty()){

    }
  }

  //! Return to top of mine
  else if(activity == Activity::OFFLOAD) pass();

  else if(activity == Activity::MINE){
    assert(target != NULL, "Worker target not set");
    target->progress +=
        GAME_TICK / (BASE_MINING_TIME + MINING_SCALE * block->loc.z);
    if(target->progress >= 100.0){
      // Choose random mineral from block to mine
      int n = 0, r = rand() % block->total();
      map<str, int>::iterator it;
      for(it = block->minerals.begin(); it != block->minerals.end(); ++it){
        n += it->second;
        if(n > r){
          m = it->first;
          break; } }
      // Add mineral to worker inventory
      map<str, int>::iterator it2;
      if((it2 = inventory.find(it->first)) == inventory.end())
        inventory[it->first] = 1;
      else ++it2;
      // Remove mineral from block
      --it->second;
      if(it->second == 0) block->minerals.erase(it);
      // Resume search unless inventory full
      target->progress = 0.0;
      if(inventory_size() == WORKER_LOAD_SIZE) activity = Activity::OFFLOAD;
      else activity = Activity::SEARCH; } } }

#endif

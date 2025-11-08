// MINE FEATURE

#ifndef MINE_HH
#define MINE_HH

#include "../planet/planet.hh"

struct Mine : Feature {

  Block* top;
  vec<Worker*> workers; //?

  Mine(){ type = MINE; }

  virtual void validate(){
    Feature::validate(); }

  void update(Planet* planet){
    // for(Worker* worker : workers){
    //   if(worker->activity == Activity::IDLE){
    //     worker->target = nodes.front();
    //     nodes.pop();
    //     graph<Block*> g;
    //     queue<Block*> q;
    //     uset<Block*> v; //? remove?
    //     point p = worker->loc;
    //     Block* b0 = &planet->earth[(int)p.x][(int)p.y][(int)p.z];
    //     g.insert(b0);
    //     q.push(b0);
    //     v.insert(b0);

    //     // Construct mine graph
    //     while(!q.empty()){
    //       Block* bt = q.front();
    //       q.pop();
    //       for(Block* b : bt->adjacent){
    //         if(!b->tunnel || v.find(b) != v.end()) continue;
    //         g.insert(b);
    //         g.link(bt, b, 1);
    //         q.push(b);
    //         v.insert(b); } }

    //     // Shortest path to target

    //     worker->activity = Activity::TRAVEL; } }
  } };

#endif

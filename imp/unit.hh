// UNIT

#ifndef UNIT_HH
#define UNIT_HH

#include "../gl/obj/point.hh"

struct Tile;
struct Block;
struct Planet;

struct Unit {
  int id, health, speed, energy;
  str name;
  point loc;

  Unit(){}
  virtual void update(Planet* p) = 0; };

enum Activity {
  IDLE,
  DIG,
  TRAVEL,
  OFFLOAD,
  MINE,
  ATTACK };

struct Worker : Unit {
  int path_index;
  Block* target;
  Activity activity;
  map<str, int> inventory;
  vec<Block*> path;

  Worker(){}
  void update(Planet* p);
  int inventory_size(){
    int r = 0;
    map<str, int>::iterator it;
    for(it = inventory.begin(); it != inventory.end(); ++it)
      r += it->second;
    return r; } };

struct Bot : Unit {
  map<str, int> gems;

  Bot(){}
  void update(Planet* p){
    //!
  } };

#endif

// PLANET BLOCK

#ifndef BLOCK_HH
#define BLOCK_HH

#include "../../../gl/object.hh"

struct Unit;

struct Block : virtual object {

  //! adjust members to describe mods
  bool tunnel;
  double progress;
  point loc;
  map<str, int> minerals;
  vec<Block*> adjacent;
  vec<Unit*> units;

  Block(){
    type = "Block"; }

  int total(){
    int r = 0;
    map<str, int>::iterator it;
    for(it = minerals.begin(); it != minerals.end(); ++it)
      r += it->second;
    return r; } };

#endif

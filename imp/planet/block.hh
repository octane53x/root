// PLANET BLOCK

#ifndef BLOCK_HH
#define BLOCK_HH

#include "../unit/unit.hh"

struct Block {

  bool tunnel;
  double progress;
  point loc;
  map<str, int> minerals;
  vec<Block*> adjacent;
  vec<Unit*> units;

  Block(){}

  int total(){
    int r = 0;
    map<str, int>::iterator it;
    for(it = minerals.begin(); it != minerals.end(); ++it)
      r += it->second;
    return r; } };

#endif

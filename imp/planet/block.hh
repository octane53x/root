// PLANET BLOCK

#ifndef BLOCK_HH
#define BLOCK_HH

#include "../unit/unit.hh"

struct Block : object {

  //! adjust members to describe mods
  bool tunnel;
  double progress;
  point loc;
  map<str, int> minerals;
  vec<Block*> adjacent;
  vec<Unit*> units;

  Block(){}

  virtual void validate(){}

  int total(){
    int r = 0;
    map<str, int>::iterator it;
    for(it = minerals.begin(); it != minerals.end(); ++it)
      r += it->second;
    return r; }

  virtual point update(double ms){ return point(0, 0); }

  void draw(image* bkgd, viewport view){} };

#endif

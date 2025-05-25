// MOVE

#ifndef MOVE_HH
#define MOVE_HH

#include "scene.hh"

void object::move(int ms){
  int dist = (int)floor(vel * ((double)ms / 1000.0));
  if(mov.type == mov_type::ROOT) return;
  if(mov.type == mov_type::PATH){

  }
}

// struct move_node {

// };

void scene::move_objs(int ms){
  //!
}

#endif

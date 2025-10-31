// TOWER FEATURE

#ifndef TOWER_HH
#define TOWER_HH

#include "../planet/planet.hh"

struct Tower : Feature {

  int damage, range;
  graph<point> border;
  map<str, int> gems;
  Unit* target;

  Tower(int _x, int _y){
    loc = point(_x, _y);
    size = point(TOWER_SIZE, TOWER_SIZE); }

  void set_type(){ type = FeatureType::TOWER; }

  void update(Planet* p){
    //!
  } };

#endif

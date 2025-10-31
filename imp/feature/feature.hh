// ABSTRACT FEATURE

#ifndef FEATURE_HH
#define FEATURE_HH

#include "../../gl/obj/point.hh"

struct Planet;

enum FeatureType {
  TREE,
  TOWER,
  MINE,
  FARM,
  FACTORY,
  LAB
};

struct Feature {

  FeatureType type;
  llu id;
  static llu next_id;

  int health;
  point loc, size;

  Feature(){
    id = new_id();
    set_type(); }

  llu new_id(){ return next_id++; }

  virtual void set_type() = 0;

  virtual void update(Planet* p) = 0; };

llu Feature::next_id = 1;

#endif

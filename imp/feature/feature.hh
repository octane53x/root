// ABSTRACT FEATURE

#ifndef FEATURE_HH
#define FEATURE_HH

#include "../../gl/model.hh"
#include "../mech/gem.hh"

struct Planet;

struct Feature : model {

  enum Type {
    TREE,
    TOWER,
    MINE,
    FARM,
    FACTORY,
    LAB };

  Type type;
  int health;
  umap<str, Gem> gems; // key = gem name

  Feature(){}

  virtual void validate(){
    model::validate(); }

  virtual void game_update(Planet* planet) = 0; };

#endif

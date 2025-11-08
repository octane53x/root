// ABSTRACT UNIT

#ifndef UNIT_HH
#define UNIT_HH

#include "../mech/item.hh"

struct Planet;

struct Unit : model {

  enum Type {
    WORKER,
    BOT,
    SHIP };

  Type type;
  int health, speed, energy;
  umap<Item, int> inventory;

  Unit(){}

  virtual void validate(){
    model::validate(); }

  virtual void game_update(Planet* planet) = 0; };

#endif

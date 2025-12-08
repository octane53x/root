// ABSTRACT ENTITY

#ifndef FEATURE_HH
#define FEATURE_HH

#include "../../gl/model.hh"
#include "../mech/gem.hh"

// Any object that requires space, e.g. building or vehicle
struct Entity : model {

  // Entity destroyed if health reduced to zero
  int health;
  // Equipped gems that enhance stats and actions
  umap<str, Gem> gems; // key = gem name
  // Location of entity, cannot be NULL
  Location* loc;

  Entity();

  virtual void validate(str func);
  virtual void update_game() = 0; };

// Ensure valid state
void Entity::validate(str func){
  model::validate(func);
  assert(health > 0, "Entity.health not positive");
  assert(loc != NULL, "Entity.Location is NULL"); }

// Set default member state
Entity::Entity(): type("Entity"), health(100) {}

#endif

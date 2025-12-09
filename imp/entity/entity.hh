// ABSTRACT ENTITY

#ifndef FEATURE_HH
#define FEATURE_HH

#include "../../gl/model.hh"
#include "../location/location.hh"
#include "../mech/gem.hh"

// Any object that requires space, e.g. building or vehicle
struct Entity : virtual model {

  // Entity destroyed if health reduced to zero
  int health;
  // Location of entity, cannot be NULL
  Location* loc;
  // Equipped gems that enhance stats and actions
  umap<str, Gem> gems; // key = gem name

  Entity();

  virtual void validate(const str& func);
  virtual void update_game() = 0; };

// Set default member state
Entity::Entity(): health(100) {
  type = "Entity"; }

// Ensure valid state
void Entity::validate(const str& func){
  model::validate(func);
  assert(health > 0, func, "Entity.health not positive");
  assert(loc != NULL, func, "Entity.Location is NULL"); }

#endif

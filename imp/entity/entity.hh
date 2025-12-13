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

  Entity(Location* _loc);
  virtual ~Entity();

  virtual void validate(const str& func);
  virtual void update_game(const double tick) = 0; };

// Set default member state
Entity::Entity(Location* _loc): health(100), loc(_loc) {
  type = "Entity";
  loc->entities[id] = this; }

// Delete from location
Entity::~Entity(){
  loc->entities.erase(id); }

// Ensure valid state
void Entity::validate(const str& func){
  model::validate(func);
  assert(health > 0, func, "Entity.health not positive");
  assert(loc != NULL, func, "Entity.Location is NULL"); }

#endif

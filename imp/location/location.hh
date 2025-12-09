// ABSTRACT LOCATION

#ifndef LOCATION_HH
#define LOCATION_HH

#include "../../gl/object.hh"

struct Entity;

// Locations hold entities, units, and other locations
struct Location : virtual object {

  // Used to decide which scene to display when not chosen by the user
  enum LocationLevel {
      INSTANCE, ENTITY, PLANET, SYSTEM, SECTOR, GALAXY, CLUSTER, UNIVERSE };

  // Location tier
  LocationLevel level;
  // Containing location
  Location* outside;
  // Locations contained within
  umap<llu, Location*> inside;
  // Entities directly within this location
  umap<llu, Entity*> entities;

  virtual void validate(const str& func);
  virtual void update_game() = 0; };

// Ensure valid state
void Location::validate(const str& func){
  object::validate(func);
  assert(!(level != Location::INSTANCE && level != Location::UNIVERSE
      && outside == NULL), func, "location.outside is NULL"); }

#endif

// ABSTRACT LOCATION

#ifndef LOCATION_HH
#define LOCATION_HH

#include "../entity/entity.hh"

struct Location {

  // Used to decide which scene to display when not chosen by the user
  enum LocationLevel {
      INSTANCE, ENTITY, PLANET, SYSTEM, SECTOR, GALAXY, CLUSTER, UNIVERSE };

  LocationLevel level;
  Location* outside;
  umap<llu, Location*> inside;
  umap<llu, Entity*> entities;

  virtual void update_game() = 0; };

#endif

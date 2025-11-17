// TERRAIN GENERATOR

#ifndef TERRAIN_HH
#define TERRAIN_HH

#include "../../gl/polygon.hh"
#include "chunk.hh"

const double
    LAND_RATIO_MIN = 0.1,
    LAND_RATIO_MAX = 0.5;

struct Terrain : thing {

  vec<polygon> land;

  virtual void validate(){}

  void gen_planet(int planet_size){

    // Coast
    double area = 0.0;
    double area_min = (double)planet_size * planet_size * LAND_RATIO_MIN;
    double area_max = (double)planet_size * planet_size * LAND_RATIO_MAX;
    double area_goal = lrand() % (llu)round(area_max - area_min) + area_min;
    while(area < area_goal){

    }

  }

  void gen_chunk(Chunk* chunk){
    srand(chunk->seed);
    //!
  } };

#endif

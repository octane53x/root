// TERRAIN GENERATOR

#ifndef TERRAIN_HH
#define TERRAIN_HH

#include "../../../gl/polygon.hh"
#include "chunk.hh"

// Side length of square chunk in tiles
const int CHUNK_SIZE = 1000;
const double
    // Bounds for how much of the surface will be land
    LAND_RATIO_MIN = 0.2,
    LAND_RATIO_MAX = 0.5;

struct Terrain : virtual system {

  // Land masses
  vec<polygon> land;

  Terrain();

  void gen_planet(const int planet_size);
  void gen_chunk(Chunk* chunk); };

// Set default member state
Terrain::Terrain(){
  type = "Terrain"; }

// Generate the planet surface features
// Called by: Planet.init
void Terrain::gen_planet(const int planet_size){
  // Land (boxes for now)
  double area = 0.0;
  double area_min = (double)planet_size * planet_size * LAND_RATIO_MIN;
  double area_max = (double)planet_size * planet_size * LAND_RATIO_MAX;
  double area_limit = lrand() % (llu)round(area_max - area_min) + area_min;
  double area_goal = area_limit * 0.8;
  while(area < area_goal){
    polygon poly;

    // Select point (not inside other land)
    point p;
    bool found = false;
    while(!found){
      p.x = (double)(lrand() % planet_size);
      p.y = (double)(lrand() % planet_size);
      bool valid = true;
      for(int i = 0; i < land.size(); ++i)
        if(land[i].inside(p)){ valid = false; break; }
      if(valid) found = true; }

    // Select other corner until no overlap and area_limit not exceeded
    bool valid = false;
    while(!valid){
      poly.points.clear();
      poly.add(p);
      point p2;
      p2.x = (double)(lrand() % planet_size);
      p2.y = (double)(lrand() % planet_size);
      poly.add(point(p.x, p2.y));
      poly.add(p2);
      poly.add(point(p2.x, p.y));
      bool valid2 = true;
      for(int i = 0; i < land.size(); ++i)
        if(land[i].intersects(poly)){
          valid2 = false; break; }
      if(!valid2) continue;
      if(area + poly.area() < area_limit) valid = true; }

    land.pb(poly);
    area += poly.area(); }

  //! Mountains, Rivers, Lakes
  validate("Terrain.gen_planet"); }

// Generate chunk tiles
// Called by: Game.init
void Terrain::gen_chunk(Chunk* chunk){
  assert(chunk != NULL, "Terrain.gen_chunk", "terrain.gen_chunk chunk is NULL");
  srand(chunk->seed);
  // Create all tiles as water
  chunk->tiles.clear();
  for(int i = 0; i < CHUNK_SIZE; ++i){
    chunk->tiles.pb(vec<Tile::Type>());
    for(int j = 0; j < CHUNK_SIZE; ++j)
      chunk->tiles[i].pb(Tile::WATER); }

  // Find land appearing in this chunk
  polygon cbox;
  cbox.add(chunk->pos);
  cbox.add(point(chunk->pos.x + CHUNK_SIZE, chunk->pos.y));
  cbox.add(point(chunk->pos.x + CHUNK_SIZE, chunk->pos.y + CHUNK_SIZE));
  cbox.add(point(chunk->pos.x, chunk->pos.y + CHUNK_SIZE));
  vec<polygon*> cland;
  for(int i = 0; i < land.size(); ++i)
    if(land[i].intersects(cbox))
      cland.pb(&land[i]);

  // Set land tiles
  //! only works for box land
  for(int i = 0; i < cland.size(); ++i){
    point topleft, botright;
    topleft = botright = cland[i]->points[0];
    for(int j = 1; j < cland[i]->points.size(); ++j){
      point p = cland[i]->points[j];
      if(p.x < topleft.x || p.y < topleft.y)
        topleft = p;
      if(p.x > botright.x || p.y > botright.y)
        botright = p; }
    for(double x = max(topleft.x, chunk->pos.x);
        dlt(x, min(botright.x, chunk->pos.x + chunk->size)); x += 1.0)
      for(double y = max(topleft.y, chunk->pos.y);
          dlt(y, min(botright.y, chunk->pos.y + chunk->size)); y += 1.0){
        if(!chunk->in_chunk(point(x, y))) //! remove
          err("terrain.gen_chunk", "find_tile range");
        *chunk->find_tile(point(x, y)) =
            (rand() % 2) ? Tile::GRASS : Tile::SNOW; } }
  validate("Terrain.gen_chunk"); }

#endif

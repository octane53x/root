// TERRAIN GENERATOR

#ifndef TERRAIN_HH
#define TERRAIN_HH

#include "../../gl/polygon.hh"
#include "chunk.hh"

const double
    LAND_RATIO_MIN = 0.2,
    LAND_RATIO_MAX = 0.5;

struct Terrain : thing {

  vec<polygon> land;

  virtual void validate(){}

  void gen_planet(int planet_size){

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
        poly.points.pb(p);
        point p2;
        p2.x = (double)(lrand() % planet_size);
        p2.y = (double)(lrand() % planet_size);
        poly.points.pb(point(p.x, p2.y));
        poly.points.pb(p2);
        poly.points.pb(point(p2.x, p.y));
        bool valid2 = true;
        for(int i = 0; i < land.size(); ++i)
          if(land[i].intersects(poly)){
            valid2 = false; break; }
        if(!valid2) continue;
        if(area + poly.area() < area_limit) valid = true; }

      land.pb(poly);
      area += poly.area(); }

    //! Mountains, Rivers, Lakes
  }

  void gen_chunk(Chunk* chunk){
    assert(chunk != NULL, "terrain.gen_chunk chunk is NULL");
    srand(chunk->seed);
    chunk->tiles.clear();
    for(int i = 0; i < CHUNK_SIZE; ++i){
      chunk->tiles.pb(vec<Tile>());
      for(int j = 0; j < CHUNK_SIZE; ++j){
        Tile tile;
        tile.pos = point(chunk->pos.x + j, chunk->pos.y + i);
        tile.type = Tile::WATER;
        tile.chunk = chunk;
        chunk->tiles[i].pb(tile); } }

    polygon cbox;
    cbox.points.pb(chunk->pos);
    cbox.points.pb(point(chunk->pos.x + CHUNK_SIZE, chunk->pos.y));
    cbox.points.pb(point(chunk->pos.x + CHUNK_SIZE, chunk->pos.y + CHUNK_SIZE));
    cbox.points.pb(point(chunk->pos.x, chunk->pos.y + CHUNK_SIZE));
    vec<polygon*> cland;
    for(int i = 0; i < land.size(); ++i)
      if(land.intersects(cbox))
        cland.pb(&land);
    for(int i = 0; i < chunk->tiles.size(); ++i)
      for(int j = 0; j < chunk->tiles[i].size(); ++j)
        for(int k = 0; k < cland.size(); ++k)
          if(cland[k]->inside(point(chunk->pos.x + j + 0.5,
              chunk->pos.y + i + 0.5))){
            chunk->tiles[i][j].type = (rand() % 2) ? Tile::GRASS : Tile::SNOW;
            break; } } };

#endif

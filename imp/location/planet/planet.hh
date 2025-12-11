// PLANET

#ifndef PLANET_HH
#define PLANET_HH

#include "../location.hh"
#include "terrain.hh"
#include "earth.hh"

const int
    // Planet size bounds, side of square surface in tiles
    PLANET_SIZE_MIN = 10000,
    PLANET_SIZE_MAX = 1000000;

// Planet or moon
struct Planet : virtual Location {

  // Side length of square surface
  int size;
  // Planet's tile surface
  Terrain terrain;
  // Planet's underground
  Earth earth;
  // All the planet's chunks, most without loaded terrain tiles
  vec<vec<Chunk> > chunks; // chunks[i][j]: i runs N to S, j runs W to E

  Planet();

  virtual void validate(const str& func);
  virtual void init();
  virtual void draw(image* canvas, const viewport& view);
  virtual void update_game();

  Chunk* find_chunk(const point& p); };

// Set default member state
Planet::Planet(){
  type = "Planet", level = Location::PLANET; }

// Ensure valid state
void Planet::validate(const str& func){
  object::validate(func);
  terrain.validate(func);
  earth.validate(func);
  assert(size >= PLANET_SIZE_MIN && size <= PLANET_SIZE_MAX, func,
      "planet.size outside bounds");
  assert(!chunks.empty(), func, "planet.chunks empty"); }

// Inititalize chunks and generate planet
// Called by: Game.init
void Planet::init(){
  size = (lrand() % ((PLANET_SIZE_MAX - PLANET_SIZE_MIN) / CHUNK_SIZE + 1))
      * CHUNK_SIZE + PLANET_SIZE_MIN;
  // Create chunks
  int nchunks = size / CHUNK_SIZE;
  double x = 0.0;
  for(int i = 0; i < nchunks; ++i){
    chunks.pb(vec<Chunk>());
    double y = 0.0;
    for(int j = 0; j < nchunks; ++j){
      Chunk c;
      c.pos = point(x, y);
      c.size = CHUNK_SIZE;
      c.seed = (ui)lrand();
      chunks[i].pb(c);
      y += CHUNK_SIZE; }
    x += CHUNK_SIZE; }
  // Connect chunks
  for(int i = 0; i < chunks.size(); ++i)
    for(int j = 0; j < chunks[i].size(); ++j){
      if(i > 0) chunks[i][j].N = &chunks[i-1][j];
      if(i < nchunks-1) chunks[i][j].S = &chunks[i+1][j];
      if(j > 0) chunks[i][j].W = &chunks[i][j-1];
      if(j < nchunks-1) chunks[i][j].E = &chunks[i][j+1]; }
  // Generate planet
  terrain.gen_planet(size); //!! Time
  earth.gen_planet(size); }

// Draw onto image
void Planet::draw(image* canvas, const viewport& view){
  //! draw
}

// Update game elements
void Planet::update_game(){
  //! grow new trees, etc
}

// Return a chunk given an environment coordinate
// Called by: Game.init
Chunk* Planet::find_chunk(const point& p){
  int i = (int)floor(p.x / CHUNK_SIZE);
  int j = (int)floor(p.y / CHUNK_SIZE);
  return &chunks[i][j]; }

#endif

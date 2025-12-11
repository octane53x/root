// CHUNK

#ifndef CHUNK_HH
#define CHUNK_HH

#include "../../../gl/object.hh"

// A 1x1 Terrain Tile, defining the surface of a Planet
// Generated on the order of N^2, so storage size is kept minimal
namespace Tile{
enum Type : uchar { WATER, GRASS, SNOW, CLIFF, BUSH, COAST, SWAMP }; }

// Planet chunk, needed due to planets being too large to load all tiles
struct Chunk : virtual object {

  // Set to CHUNK_SIZE
  int size;
  // RNG seed for generating tiles consistently
  ui seed;
  // Tile types in the chunk
  vec<vec<Tile::Type> > tiles; // tiles[i][j]: i runs N to S, j runs W to E
  // Linked chunks in all four directions
  Chunk* N, *S, *E, *W;

  Chunk();

  Chunk& operator=(const Chunk& o);

  virtual void validate(const str& func);
  virtual void draw(image* canvas, const viewport& view);

  bool in_chunk(const point& p) const;

  Tile::Type* find_tile(const point& p); };

// Set default member state
Chunk::Chunk(): N(NULL), S(NULL), E(NULL), W(NULL) {
  type = "Chunk"; }

// Assignment operator
Chunk& Chunk::operator=(const Chunk& o){
  pos = o.pos;
  size = o.size, seed = o.seed, tiles = o.tiles;
  N = o.N, S = o.S, E = o.E, W = o.W;
  validate("Chunk.operator=");
  return *this; }

// Ensure valid state
void Chunk::validate(const str& func){
  object::validate(func);
  assert(size > 0, func, "chunk.size is zero");
  assert(seed != 0, func, "chunk.seed is zero");
  assert(!(N == NULL && S == NULL && E == NULL && W == NULL), func,
      "All linked chunks are NULL"); }

// Draw onto an image
void Chunk::draw(image* canvas, const viewport& view){
  //! draw
}

// Is coordinate in chunk
bool Chunk::in_chunk(const point& p) const {
  return p.x >= pos.x && p.x <= pos.x + size
      && p.y >= pos.y && p.y <= pos.y + size; }

// Return the tile type at the given coordinate
Tile::Type* Chunk::find_tile(const point& p){
  assert(in_chunk(p), "Chunk.find_tile", "Chunk.find_tile point not in chunk");
  int i = (int)floor(p.y - pos.y), j = (int)floor(p.x - pos.x);
  return &tiles[i][j]; }

#endif

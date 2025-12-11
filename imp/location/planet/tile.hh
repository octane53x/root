// TILE

#ifndef TILE_HH
#define TILE_HH

// A 1x1 Terrain Tile, defining the surface of a Planet
// Generated on the order of N^2, so storage size is kept minimal
namespace Tile{
enum Type : uchar { WATER, GRASS, SNOW, CLIFF, BUSH, COAST, SWAMP }; }

// Textures associated with tiles
umap<Tile::Type, color> TILE_TEX = {
    {Tile::WATER, BLUE},
    {Tile::GRASS, LIME},
    {Tile::SNOW, WHITE},
    {Tile::CLIFF, BROWN},
    {Tile::BUSH, GREEN},
    {Tile::COAST, YELLOW},
    {Tile::SWAMP, BLACK} };

#endif

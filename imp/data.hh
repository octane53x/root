// DATA

#ifndef DATA_HH
#define DATA_HH

#include "../gl/obj.hh"

const double // in seconds
  GAME_TICK = 0.1,
  BASE_MINING_TIME = 10.0,
  MINING_SCALE = 0.005, // per depth
  DIG_TIME = 30.0,
  TRAVEL_TIME = 1.0; // per block

const int
  TOWER_SIZE = 5,
  MINE_SIZE = 3,
  FACTORY_SIZE = 4,

  SURFACE_WIDTH = 5000,

  MINE_WIDTH = 201,
  MINE_DEPTH = 100,
  MIN_MINERALS = 3,
  MAX_MINERALS = 10,
  MIN_BLOCKS = 10,
  MAX_BLOCKS = 10000,

  WORKER_LOAD_SIZE = 100;

const point PLAYER_START = point(0, 0);

const int C_MINERALS = 3;
const vec<str> MINERALS = {
  // CUSTOM:
  "SOIL",
  "SAND",
  "STONE",
  // RANDOM:
  "COAL",
  "IRON",
  "COPPER",
  "ALUMINUM",
  "LIMESTONE",
  "GRANITE",
  "MARBLE",
  "LEAD",
  "TIN",
  "ZINC",
  "NICKEL",
  "GYPSUM",
  "LITHIUM",
  "SULFUR",
  "PHOSPHORUS",
  "SILICON",
  "TITANIUM",
  "SILVER",
  "GOLD",
  "PLATINUM",
  "ADAMANTIUM",
  "URANIUM",
  "NEPTUNIUM",
  "PLUTONIUM" };

const vec<str> GEMS = {
  "AMETHYST", // A
  "TOURMALINE", // B
  "JASPER", // C
  "DIAMOND", // D
  "EMERALD", // E
  "MALACHITE", // F
  "JADE", // G
  "AMAZONITE", // H
  "ZIRCON", // I
  "LAPIS LAZULI", // J
  "OBSIDIAN", // K
  "GASPEITE", // L
  "HEMATITE", // M
  "CHAROITE", // N
  "AMBER", // O
  "AQUAMARINE", // P
  "TURQUOISE", // Q
  "GARNET", // R
  "SAPPHIRE", // S
  "PEARL", // T
  "AZURITE", // U
  "RHODONITE", // V
  "QUARTZ", // W
  "ONYX", // X
  "TANZANITE", // Y
  "TOPAZ", // Z
  "OPAL", // 0
  "RUBY", // 1
  "CARNELIAN", // 2
  "AGATE", // 3
  "PERIDOT", // 4
  "CITRINE" // 5
};

#endif

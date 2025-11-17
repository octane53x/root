// DATA

#ifndef DATA_HH
#define DATA_HH

#include "../gl/obj/point.hh"

const double // in seconds
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

const point PLAYER_START = point(0, 0, 0);

#endif

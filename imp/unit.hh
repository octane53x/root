// UNIT

#ifndef UNIT_HH
#define UNIT_HH

#include "../gl/gl_obj.hh"

struct Unit : thing {
  int id, health, speed, energy;
  str name;
  point loc;
  Unit(){ type = "Unit"; } };

struct Worker : Unit {
  int progress;
  Worker(){ type = "Worker"; } };

struct Bot : Unit {
  map<str, int> gems;
  Bot(){ type = "Bot"; } };

#endif

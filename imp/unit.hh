// UNIT

#ifndef UNIT_HH
#define UNIT_HH

#include "../core/obj.hh"

struct Unit {
  int id, health, speed, energy;
  str name;
  point loc;
  Unit(){} };

struct Worker : Unit {
  int progress;
  Worker(){} };

struct Bot : Unit {
  map<str, int> gems; };

#endif

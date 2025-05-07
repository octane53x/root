// UNIT

#ifndef UNIT_HH
#define UNIT_HH

#include "imp_incl.hh"

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

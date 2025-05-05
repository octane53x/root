// UNIT

#ifndef UNIT_HH
#define UNIT_HH

#include "../lang/lang.hh"

struct Unit {
  int id, health, speed, energy;
  str name;
  point loc;
  Unit(){
    id = g.next_id(); } };

struct Worker : Unit {
  int progress;
  Worker(){} };

struct Bot : Unit {
  map<str, int> gems; };

#endif

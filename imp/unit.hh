// UNIT

#ifndef UNIT_HH
#define UNIT_HH

#include "../gl/obj.hh"

struct Unit {
  int id, health, speed, energy;
  str name;
  point loc;
  Unit(){}
  virtual void update() = 0; };

struct Worker : Unit {
  int progress;
  Worker(){}
  void update(){
    //!
  } };

struct Bot : Unit {
  map<str, int> gems;
  Bot(){}
  void update(){
    //!
  } };

#endif

// FEATURE

#ifndef FEATURE_HH
#define FEATURE_HH

#include "data.hh"
#include "unit.hh"
#include "recipe.hh"

struct Tile;
struct Block;
struct Planet;

struct Feature {
  int health;
  point loc, size;

  Feature(){}
  virtual void update(Planet* p) = 0; };

struct Tree : Feature {
  int wood;

  Tree(){}
  void update(Planet* p){
    //!
  } };

struct Tower : Feature {
  int damage, range;
  graph<point> border;
  map<str, int> gems;
  Unit* target;

  Tower(int _x, int _y){
    loc = point(_x, _y);
    size = point(TOWER_SIZE, TOWER_SIZE); }
  void update(Planet* p){
    //!
  } };

struct Mine : Feature {
  Block* top;
  vec<Worker*> workers;
  queue<Block*> nodes;
  map<str, int> gems;

  Mine(int _x, int _y){
    loc = point(_x, _y);
    size = point(MINE_SIZE, MINE_SIZE); }
  void update(Planet* p); };

struct Farm : Feature {
  graph<point> border;
  vec<vec<str> > crops;
  vec<Worker*> workers;
  map<str, int> gems;

  Farm(){}
  void update(Planet* p){
    //!
  } };

struct Factory : Feature {
  Recipe recipe;
  map<str, int> gems;

  Factory(int _x, int _y){
    loc = point(_x, _y);
    size = point(FACTORY_SIZE, FACTORY_SIZE); }
  void update(Planet* p){
    //!
  } };

struct Lab : Feature {
  map<str, int> gems;

  Lab(){}
  void update(Planet* p){
    //!
  } };

#endif

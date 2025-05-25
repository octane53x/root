// FEATURE

#ifndef FEATURE_HH
#define FEATURE_HH

#include "data.hh"
#include "unit.hh"
#include "recipe.hh"

struct Block;

struct Feature {
  int health;
  point loc, size;
  Feature(){} };

struct Tree : Feature {
  int wood;
  Tree(){} };

struct Tower : Feature {
  int damage, range;
  graph<point> border;
  map<str, int> gems;
  Tower(int _x, int _y){
    loc = point(_x, _y);
    size = point(TOWER_SIZE, TOWER_SIZE); } };

struct Mine : Feature {
  Block* top;
  vec<Worker> workers;
  map<str, int> gems;
  Mine(int _x, int _y){
    loc = point(_x, _y);
    size = point(MINE_SIZE, MINE_SIZE); } };

struct Farm : Feature {
  graph<point> border;
  vec<vec<str> > crops;
  vec<Worker> workers;
  map<str, int> gems;
  Farm(){} };

struct Factory : Feature {
  Recipe recipe;
  map<str, int> gems;
  Factory(int _x, int _y){
    loc = point(_x, _y);
    size = point(FACTORY_SIZE, FACTORY_SIZE); } };

struct Lab : Feature {
  map<str, int> gems;
  Lab(){} };

#endif

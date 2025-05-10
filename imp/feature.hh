// FEATURE

#ifndef FEATURE_HH
#define FEATURE_HH

#include "data.hh"
#include "unit.hh"
#include "recipe.hh"

struct Block;

struct Feature : thing {
  int health;
  point loc, size;
  Feature(){ type = "Feature"; } };

struct Tree : Feature {
  int wood;
  Tree(){ type = "Tree"; } };

struct Tower : Feature {
  int damage, range;
  graph<point> border;
  map<str, int> gems;
  Tower(int _x, int _y){
    type = "Tower";
    loc = point(_x, _y);
    size = point(TOWER_SIZE, TOWER_SIZE); } };

struct Mine : Feature {
  Block* top;
  vec<Worker> workers;
  map<str, int> gems;
  Mine(int _x, int _y){
    type = "Mine";
    loc = point(_x, _y);
    size = point(MINE_SIZE, MINE_SIZE); } };

struct Farm : Feature {
  graph<point> border;
  vec<vec<str> > crops;
  vec<Worker> workers;
  map<str, int> gems;
  Farm(){ type = "Farm"; } };

struct Factory : Feature {
  Recipe recipe;
  map<str, int> gems;
  Factory(int _x, int _y){
    type = "Factory";
    loc = point(_x, _y);
    size = point(FACTORY_SIZE, FACTORY_SIZE); } };

struct Lab : Feature {
  map<str, int> gems;
  Lab(){ type = "Lab"; } };

#endif

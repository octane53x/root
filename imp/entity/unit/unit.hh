// ABSTRACT UNIT

#ifndef UNIT_HH
#define UNIT_HH

#include "../entity.hh"
#include "../../location/planet/planet.hh"
#include "../../mech/item.hh"

struct Ship;

// An entity with movement, able to board ships
struct Unit : virtual Entity {

  // Available energy used for activities like combat
  int energy;
  // Travel speed, can be altered by game effects
  double speed; // tiles per second
  // Plan for movement, determined when destination is selected
  queue<point> path;
  // Ship on which the unit is boarded, can be NULL
  Ship* ship;
  // Inventory of items carried by the unit
  umap<str, Item> inventory; // key = item name

  Unit();

  virtual void validate(const str& func);
  virtual void update(const double ms);

  void move_dist(const double dist_able);
  void move(const point& p); };

// Set default member state
Unit::Unit(): speed(1.0), ship(NULL) {
  type = "Unit"; }

// Ensure valid state
void Unit::validate(const str& func){
  Entity::validate(func);
  assert(dgeq(speed, 0.0), func, "Unit.speed negative"); }

// Update to the next frame
// Called by: scene.update -> virtual object.update
void Unit::update(const double ms){
  double dist_able = ms * speed / 1000.0;
  move_dist(dist_able);
  validate("Unit.update"); }

// Move the unit as far as it can go along its path
// Rarely if ever should a unit be stationary
// Called by: update
void Unit::move_dist(const double dist_able){
  if(deq(dist_able, 0.0) || path.empty()) return;
  double dist_avail = dist_able;
  point prog(0, 0);
  while(dgt(dist_avail, 0.0) && !path.empty()){
    point next = path.front();
    double dist_next = line(pos, next).len();
    if(deq(dist_next, 0.0)){
      path.pop();
      continue; }
    double dist = min(dist_avail, dist_next);
    double ratio = dist / dist_next;
    prog += (next - pos) * ratio;
    dist_avail -= dist; }

  // Load/unload chunks
  //!

  last_move = prog;
  validate("Unit.move_dist"); }

// Create a path to a coordinate
// Called by TileBtn.click
void Unit::move(const point& p){
  if(loc->type != "Planet") return;
  path = queue<point>();
  path.push(p);
  validate("Unit.move"); }

#endif

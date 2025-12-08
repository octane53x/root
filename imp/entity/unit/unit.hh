// ABSTRACT UNIT

#ifndef UNIT_HH
#define UNIT_HH

#include "../entity.hh"
#include "../mech/item.hh"

struct Ship;

// An entity with movement, able to board ships
struct Unit : Entity {

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

  virtual void validate(str func);
  virtual void update(double ms);

  void move(double dist_able); };

// Set default member state
Unit::Unit(): type("Unit"), speed(1.0), ship(NULL) {}

// Ensure valid state
void Unit::validate(str func){
  Entity::validate(func);
  assert(dgeq(speed, 0.0), "Unit.speed negative"); }

// Update to the next frame
// Called by: scene.update -> virtual object.update
void Unit::update(double ms){
  double dist_able = ms * speed / 1000.0;
  move(dist_able);
  validate("Unit.update"); }

// Move the unit as far as it can go along its path
// Rarely if ever should a unit be stationary
// Called by: update
void Unit::move(double dist_able){
  if(deq(dist_able, 0.0)) return;
  point prog(0, 0);

  // Center unit on tile
  if(path.empty()){
    point dest_max(tile->pos.x + 0.5, tile->pos.y + 0.5);
    double dist_dest_max = line(pos, dest_max).len();
    if(deq(dist_dest_max, 0.0)) return point(0, 0);
    double dist = min(dist_able, dist_dest_max);
    double ratio = dist / dist_dest_max;
    prog = (dest_max - pos) * ratio;

  // Proceed on path
  }else{
    while(dgt(dist_able, 0.0) && !path.empty()){
      point next = path.front();
      double dist_next = line(pos, next).len();
      if(deq(dist_next, 0.0)){
        path.pop();
        continue; }
      double dist = min(dist_able, dist_next);
      double ratio = dist / dist_next;
      prog += (next - pos) * ratio;
      dist_able -= dist; } }

  // Load/unload chunks
  //!

  last_move = prog;
  validate("Unit.move"); }

#endif

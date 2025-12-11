// MINE ENTITY

#ifndef MINE_HH
#define MINE_HH

#include "../location/planet/planet.hh"

struct Worker;

// A mine to harvest minerals found on a planet
struct Mine : virtual Entity {

  // Entry block to the tunnels beneath
  Block* top;
  // All workers currently within the mine
  vec<Worker*> workers;

  Mine();

  virtual void draw(image* canvas, const viewport& view);
  virtual void update_game(); };

// Set default member state
Mine::Mine(): top(NULL) {
  type = "Mine"; }

// Draw onto image
void Mine::draw(image* canvas, const viewport& view){
  polygon poly;
  point p(pos.x - 1.0, pos.y - 1.0);
  poly.add(p);
  poly.add(point(p.x + 2.0, p.y));
  poly.add(point(p.x + 2.0, p.y + 2.0));
  poly.add(point(p.x, p.y + 2.0));
  poly.fill = PURPLE;
  poly.draw(canvas, view); }

// Mine operations
void Mine::update_game(){}

#endif

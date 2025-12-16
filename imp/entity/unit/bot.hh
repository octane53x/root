// BOT UNIT

#ifndef BOT_HH
#define BOT_HH

#include "unit.hh"

// Unit with combat and other abilities, playable by the user
struct Bot : virtual Unit {

  // Maximum stage unlocked (1-10)
  int stage;
  // Targeted coordinate on the terrain
  point target_point;
  // Targeted entity
  Entity* target;

  Bot(Location* _loc);

  virtual void validate(const str& func);
  virtual void draw(image* canvas, const viewport& view);
  virtual void update_game(const double tick); };

// Set default member state
Bot::Bot(Location* _loc): Entity(_loc), Unit() {
  type = "Bot";
  stage = 3; //! temp
  speed = 2.0;
  inventory_cap = 100;
  hitbox_size = point(1.0, 1.0, 1.0); }

// Ensure valid state
void Bot::validate(const str& func){
  Unit::validate(func);
  str msg("bot stage and location incompatible");
  switch(stage){
  case 1:
    assert(loc->level == Location::INSTANCE, func, msg); break;
  case 2:
    assert(loc->level == Location::ENTITY, func, msg); break;
  case 3:
  case 4:
    assert(loc->level <= Location::PLANET, func, msg); break;
  case 5:
    assert(loc->level <= Location::SYSTEM, func, msg); break;
  case 6:
    assert(loc->level <= Location::SECTOR, func, msg); break;
  case 7:
    assert(loc->level <= Location::GALAXY, func, msg); break;
  case 8:
    assert(loc->level <= Location::CLUSTER, func, msg); break;
  case 9:
  case 10:
    break;
  default:
    err("Bot.validate", "player stage outside bounds"); } }

// Draw onto image
void Bot::draw(image* canvas, const viewport& view){
  polygon poly;
  double x = pos.x - 1.0;
  double y = pos.y - 1.0;
  poly.add(point(x, y));
  poly.add(point(x + 2.0, y));
  poly.add(point(x + 2.0, y + 2.0));
  poly.add(point(x, y + 2.0));
  poly.fill = RED;
  poly.draw(canvas, view);
  validate("Bot.draw"); }

// Update game elements
void Bot::update_game(const double tick){
  //! recharge weapon, etc
}

#endif

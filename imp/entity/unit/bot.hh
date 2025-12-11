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

  Bot();

  virtual void validate(const str& func);
  virtual void draw(image* canvas, const viewport& view);
  virtual void update_game(); };

// Set default member state
Bot::Bot(): stage(3) {
  type = "Bot";
  speed = 0.5;
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
    err("player stage outside bounds"); } }

// Draw onto image
// Currently done by the scene
void Bot::draw(image* canvas, const viewport& view){}

// Update game elements
void Bot::update_game(){
  //! recharge weapon, etc
}

#endif

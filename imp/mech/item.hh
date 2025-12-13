// ITEM MECHANIC

#ifndef ITEM_HH
#define ITEM_HH

#include "../../gl/image.hh"

// Item of some count
struct Item : virtual thing {

  // Number of item
  int count;
  // Unique item name
  str name;
  // Displayed image
  image* icon;

  Item();

  virtual void validate(const str& func); };

// Required implementation
Item::Item(): count(0) {}

// Ensure valid state
void Item::validate(const str& func){
  assert(count > 0, func, "count not positive");
  assert(name != "", func, "name is empty"); }

#endif

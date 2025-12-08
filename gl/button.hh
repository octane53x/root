// ABSTRACT BUTTON

#ifndef BUTTON_HH
#define BUTTON_HH

#include "polygon.hh"

// Button you can hover and click to call functionality
// Polygon is the click hitbox
struct button : virtual polygon, virtual image {

  // Whether the button is currently being hovered over by the cursor
  bool hovered;

  button();

  virtual void validate(const str& func) const;
  virtual void draw(image* canvas, const viewport& view);
  virtual void hover_fn() = 0;
  virtual void click_fn() = 0;

  bool hover(const point& c);
  bool click(const point& c); };

// Ensure valid state
void button::validate(const str& func) const {
  polygon::validate(func);
  image::validate(func); }

// Draw the button onto the scene
// Called by: scene.draw -> virtual object.draw
void button::draw(image* canvas, const viewport& view){
  if(fill != CLEAR) polygon::draw(canvas, view);
  else if(!image::empty()) image::draw(canvas, view);
  validate("button.draw"); }

// If the cursor position sent lies over the button, call its hover function
// Called by: env.hover
bool button::hover(const point& c){
  if(hovered && inside(c))
    return true;
  if(hovered && !inside(c)){
    hovered = false;
    return false; }
  if(!hovered && inside(c)){
    hovered = true;
    hover_fn();
    validate("button.click");
    return true; }
  return false; }

// If the cursor position sent lies over the button, call its click function
// Called by: env.click
bool button::click(const point& c){
  if(inside(c)){
    click_fn();
    validate("button.click");
    return true; }
  return false; }

#endif

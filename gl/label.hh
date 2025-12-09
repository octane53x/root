// LABEL

#ifndef LABEL_HH
#define LABEL_HH

#include "image.hh"
#include "font.hh"

// The pixel spacing between characters
const int SMALL_SPACING = 2,
          LARGE_SPACING = 5;

// Object that displays text from font images
struct label : virtual object {

  // Pixel height of characters
  int size;
  // Text to display
  str text;
  // Font images to display characters
  font* font;

  label();

  virtual void validate(const str& func);
  virtual void draw(image* canvas, const viewport& view); };

// Set default memher state
label::label(): size(10), text("X"), font(NULL) {
  type = "label"; }

// Ensure valid state
void label::validate(const str& func){
  object::validate(func);
  assert(size > 0, "label text size not positive"); }

// Draw label onto image
void label::draw(image* canvas, const viewport& view){
  int x = (int)floor(pos.x);
  for(int k = 0; k < text.size(); ++k){
    image orig = font->syms[text[k]];
    image img = orig.scale((double)size / orig.height);
    img.pos = point(x, pos.y);
    img.replace_except(CLEAR, fill);
    img.draw(canvas, view);
    x += img.width + ((size >= 100) ? LARGE_SPACING : SMALL_SPACING); }
  validate("label.draw"); }

#endif

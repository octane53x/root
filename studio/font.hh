// STUDIO FONT

#ifndef FONT_HH
#define FONT_HH

#include "studio.hh"

// Split characters from font image
void Studio::load_font(){
  image font_img = load_bmp(FONT_LOC);
  for(int i = 0; i < SYMBOLS.size(); ++i){
    image c(ipoint(CHAR_WIDTH_SCALE_1, LINE_HEIGHT_SCALE_1));
    for(int xi = i * c.size.x, xo = 0; xo < c.size.x; ++xi, ++xo)
      for(int y = 0; y < c.size.y; ++y)
        c.set_pixel(ipoint(xo, y), font_img.data[y][xi]);
    font_base[SYMBOLS[i]] = c; } }

void Studio::color_font(const double scale){
  if(Panel::fonts.find(scale) != Panel::fonts.end()) return;
  umap<color, umap<color, font> > cmbkgd;
  // Code colors
  vec<color> bkgds = {BKGD_COLOR, SELECT_COLOR};
  for(const color& cb : bkgds){
    umap<color, font> cmtext;
    for(const color& ct : TEXT_COLORS){
      font f;
      for(const char c : _SYMBOLS)
        f[c] = color_char(font_base[c], ct, cb).scale(scale);
      cmtext[ct] = f; }
    cmbkgd[cb] = cmtext; }
  // File and command bar colors
  bkgds = {CMD_BAR_COLOR, FOCUS_FILE_BAR_COLOR, UNFOCUS_FILE_BAR_COLOR,
      CURSOR_COLOR};
  for(const color& cb : bkgds){
    umap<color, font> cmtext;
    font f;
    for(const char c : _SYMBOLS)
      f[c] = color_char(font_base[c], BAR_TEXT_COLOR, cb).scale(scale);
    cmtext[BAR_TEXT_COLOR] = f;
    cmbkgd[cb] = cmtext; }
  Panel::fonts[scale] = cmbkgd; }

image Studio::color_char(const image& img, const color& ctext,
    const color& cbkgd){
  image r = img;
  for(int y = 0; y < img.size.y; ++y)
    for(int x = 0; x < img.size.x; ++x){
      color ci = img.data[y][x];
      double brt = (double)((int)ci.r + ci.g + ci.b) / (255 * 3);
      color co;
      if(brt > 0.2){
        brt = min(1.0, brt * 1.2);
        co = color((uchar)floor(brt * ctext.r),
                   (uchar)floor(brt * ctext.g),
                   (uchar)floor(brt * ctext.b));
      }else
        co = color((uchar)floor((1.0 - brt) * cbkgd.r),
                   (uchar)floor((1.0 - brt) * cbkgd.g),
                   (uchar)floor((1.0 - brt) * cbkgd.b));
      r.data[y][x] = co; }
  return r; }

void Studio::scale_font(const double factor){
  Panel& p = *focus;
  Cursor& c = p.cursor;
  p.text_scale *= factor;
  c.size.y = p.line_height = (int)ceil(p.text_scale * LINE_HEIGHT_SCALE_1);
  c.size.x = p.char_width = (int)ceil(p.text_scale * CHAR_WIDTH_SCALE_1);
  color_font(p.text_scale); }

#endif

// STUDIO CLIPBOARD

#ifndef CLIP_HH
#define CLIP_HH

#include "studio.hh"

void Studio::clip(){
  Panel& p = *focus;
  Cursor& c = p.cursor;
  if(p.mark.y == -1) return;
  int y0, yf, x0, xf;
  if(c.pos.y < p.mark.y || (c.pos.y == p.mark.y && c.pos.x < p.mark.x))
    y0 = c.pos.y, yf = p.mark.y, x0 = c.pos.x, xf = p.mark.x - 1;
  else if(c.pos.y > p.mark.y || (c.pos.y == p.mark.y && c.pos.x > p.mark.x))
    y0 = p.mark.y, yf = c.pos.y, x0 = p.mark.x, xf = c.pos.x - 1;
  else return;
  clipboard.clear();
  clipboard.pb("");
  while(y0 < yf || (y0 == yf && x0 <= xf)){
    if(x0 < p.text[y0].size()){
      clipboard.back() += str(1, p.text[y0][x0]);
      ++x0;
    }else if(x0 == p.text[y0].size()){
      clipboard.pb("");
      ++y0;
      x0 = 0; } } }

void Studio::cut(){
  Panel& p = *focus;
  if(p.mark.y == -1) return;
  clip();
  p.delete_selection(); }

void Studio::copy(){
  Panel& p = *focus;
  Cursor& c = p.cursor;
  if(p.mark.y == -1) return;
  clip();
  ipoint p0, pf;
  if(p.mark.y < c.pos.y || (p.mark.y == c.pos.y && p.mark.x < c.pos.x)){
    p0 = p.mark;
    pf = c.pos;
  }else{
    p0 = c.pos;
    pf = c.pos = p.mark; }
  --pf.x;
  p.mark = ipoint(-1, -1);
  p.draw_selection(p0, pf, true); }

void Studio::paste(){
  Panel& p = *focus;
  Cursor& c = p.cursor;
  if(clipboard.empty()) return;
  p.insert_text(clipboard, c.pos);
  c.pos.y += (int)clipboard.size() - 1;
  if(clipboard.size() == 1)
    c.pos.x += (int)clipboard.back().size();
  else
    c.pos.x = (int)clipboard.back().size();
  while(c.pos.y > p.top_line + p.size.y / p.line_height)
    p.scroll(DOWN); }

#endif

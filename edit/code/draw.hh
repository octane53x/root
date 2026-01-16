// EDITOR DRAWING

#ifndef DRAW_HH
#define DRAW_HH

#include "editor.hh"

void Editor::draw(){
  updated = false;
  for(Panel& p : panels)
    p.draw();
  if(&cmd == focus)
    cmd.draw();
  else
    for(int y = cmd.pos.y; y < cmd.pos.y + cmd.size.y; ++y)
      for(int x = 0; x < cmd.size.x; ++x)
        frame.data[y][x] = BKGD_COLOR; }

void Panel::draw(){
  updated = false;
  // Draw text
  int yf = min((int)text.size() - 1,
      top_line + (int)ceil((double)size.y / line_height) - 1);
  draw_selection(ipoint(0, top_line), ipoint((int)text[yf].size() - 1, yf));
  // Draw empty space after last line
  for(int y = pos.y + max(0, (yf - top_line) * line_height);
      y < pos.y + min(size.y, (yf - top_line + 1) * line_height); ++y)
    for(int x = pos.x + (int)text[yf].size() * char_width;
        x < pos.x + size.x; ++x)
      frame->data[y][x] = bkgd;
  if(cmd) return;
  // Draw past last line of text
  for(int y = pos.y + max(0, (yf + 1 - top_line) * line_height);
      y < pos.y + size.y; ++y)
    for(int x = pos.x; x < pos.x + size.x; ++x)
      frame->data[y][x] = bkgd;
  // Draw cursor, divider, and file bar
  if(!focus)
    cursor.draw(text_to_frame(cursor.pos));
  draw_divider();
  draw_file_bar(); }

// Draw one character
void Panel::draw_char(const image& img, const ipoint& p){
  for(int yo = max(0, p.y), yi = max(0, -p.y);
      yo < frame->size.y && yi < img.size.y; ++yo, ++yi)
    for(int xo = max(0, p.x), xi = max(0, -p.x);
        xo < frame->size.x && xi < img.size.x; ++xo, ++xi)
      frame->data[yo][xo] = img.data[yi][xi]; }

void Panel::draw_selection(const ipoint& p0, const ipoint& pf){
  Cursor& c = cursor;
  // Draw whole panel if selection clears past text
  if(pf.y >= text.size()){
    draw();
    return; }

  // Find selection to determine background color
  ipoint mark0, markf;
  if(mark.y != -1){
    mark0 = (c.pos.y < mark.y || (c.pos.y == mark.y && c.pos.x < mark.x))
        ? c.pos : mark;
    markf = (c.pos == mark0) ? mark : c.pos;
    --markf.x; }

  // Draw single line
  if(p0.y == pf.y){
    for(int x = p0.x; x <= pf.x; ++x){
      color cb = (mark.y != -1 && in_selection(mark0, markf, ipoint(x, p0.y)))
          ? SELECT_COLOR : bkgd;
      draw_char(fonts[text_scale][cb][text_color[p0.y][x]][text[p0.y][x]],
          text_to_frame(ipoint(x, p0.y))); }
    return; }

  // Draw several lines
  for(int x = p0.x; x < text[p0.y].size(); ++x){
    color cb = (mark.y != -1 && in_selection(mark0, markf, ipoint(x, p0.y)))
        ? SELECT_COLOR : bkgd;
    draw_char(fonts[text_scale][cb][text_color[p0.y][x]][text[p0.y][x]],
        text_to_frame(ipoint(x, p0.y))); }
  for(int y = p0.y + 1; y <= pf.y - 1; ++y)
    for(int x = 0; x < text[y].size(); ++x){
      color cb = (mark.y != -1 && in_selection(mark0, markf, ipoint(x, y)))
          ? SELECT_COLOR : bkgd;
      draw_char(fonts[text_scale][cb][text_color[y][x]][text[y][x]],
          text_to_frame(ipoint(x, y))); }
  for(int x = 0; x <= min((int)text[pf.y].size() - 1, pf.x); ++x){
    color cb = (mark.y != -1 && in_selection(mark0, markf, ipoint(x, pf.y)))
        ? SELECT_COLOR : bkgd;
    draw_char(fonts[text_scale][cb][text_color[pf.y][x]][text[pf.y][x]],
        text_to_frame(ipoint(x, pf.y))); }

  // Draw empty space
  for(int y = p0.y; y < pf.y; ++y)
    for(int x = (int)text[y].size(); x <= PANEL_CHARS; ++x){
      color cb = (mark.y != -1 && in_selection(mark0, markf, ipoint(x, y)))
          ? SELECT_COLOR : bkgd;
      draw_char(fonts[text_scale][cb][COLOR_CODE][' '],
          text_to_frame(ipoint(x, y))); } }

void Panel::draw_divider(){
  for(int y = pos.y; y < pos.y + size.y; ++y)
    for(int x = pos.x + size.x; x < pos.x + size.x + VERTICAL_DIVIDE; ++x)
      frame->data[y][x] = DIVIDER_COLOR;
  //! draw scroll marker
}

void Panel::draw_file_bar(){
  // Draw bar
  color cbkgd = focus ? FOCUS_FILE_BAR_COLOR : UNFOCUS_FILE_BAR_COLOR;
  for(int y = pos.y + size.y; y < pos.y + size.y + LINE_HEIGHT_SCALE_1; ++y)
    for(int x = pos.x; x < pos.x + size.x + VERTICAL_DIVIDE; ++x)
      frame->data[y][x] = cbkgd;
  // Determine file bar text
  str bar_text = str(saved ? "-----" : "*****") + "     ";
  if(file.find("\\root\\") != str::npos)
    bar_text += file.substr(file.find("\\root\\") + 6) + "     ";
  bar_text += "(" + to_string(cursor.pos.y + 1) + ","
      + to_string(cursor.pos.x + 1) + ")";
  // Draw text
  for(int x = 0; x < bar_text.size(); ++x)
    draw_char(fonts[text_scale][cbkgd][BAR_TEXT_COLOR][bar_text[x]],
        ipoint(pos.x + x * char_width, pos.y + size.y)); }

// Handled by panel.update when in focus
void Cursor::draw(const ipoint& win_pos){
  updated = false;
  for(int y = win_pos.y; y < win_pos.y + size.y; ++y){
    frame->data[y][win_pos.x] = CURSOR_COLOR;
    frame->data[y][win_pos.x + size.x - 1] = CURSOR_COLOR; }
  for(int x = win_pos.x; x < win_pos.x + size.x; ++x){
    frame->data[win_pos.y][x] = CURSOR_COLOR;
    frame->data[win_pos.y + size.y - 1][x] = CURSOR_COLOR; } }

#endif

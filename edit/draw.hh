// EDITOR DRAWING

#ifndef DRAW_HH
#define DRAW_HH

#include "editor.hh"

// Do nothing since this app draws on update
bool Editor::draw(){
  return false; }

void Editor::draw_all(){
  updated = false;
  for(Panel& p : panels)
    p.draw(true);
  draw_cmd(true); }

void Editor::draw_cmd(const bool blt){
  if(&cmd_panel == focus)
    cmd_panel.draw(false);
  else
    for(int y = cmd_panel.pos.y; y < cmd_panel.pos.y + cmd_panel.size.y; ++y)
      for(int x = 0; x < cmd_panel.size.x; ++x)
        frame.data[y][x] = BKGD_COLOR;
  if(blt)
    InvalidateRect(_win->hWnd, &get_rect(cmd_panel.pos, cmd_panel.size),
        FALSE); }

void Panel::draw(const bool blt){
  updated = false;
  // Clear panel
  for(int y = pos.y; y < pos.y + size.y; ++y)
    for(int x = pos.x; x < pos.x + size.x; ++x)
      frame->data[y][x] = BKGD_COLOR;
  // Draw text
  int yf = min((int)text.size() - 1,
      top_line + (int)ceil((double)size.y / line_height) - 1);
  draw_selection(ipoint(0, top_line),
      ipoint((int)text[yf].size() - 1, yf), false);
  // Draw cursor, divider, and file bar
  if(!focus)
    cursor.draw(text_to_frame(cursor.pos), false);
  draw_divider(false);
  draw_file_bar(false);
  if(blt)
    InvalidateRect(_win->hWnd, &get_rect(pos, ipoint(size.x + VERTICAL_DIVIDE,
        size.y + LINE_HEIGHT_SCALE_1)), FALSE); }

// Draw one character
void Panel::draw_char(const image& img, const ipoint& p, const bool blt){
  ipoint tl(max(0, p.x), max(0, p.y));
  ipoint sz(min(frame->size.x - tl.x, img.size.x - max(0, -p.x)),
      min(frame->size.y - tl.y, img.size.y - max(0, -p.y)));
  for(int yo = tl.y, yi = max(0, -p.y);
      yo < frame->size.y && yi < img.size.y; ++yo, ++yi)
    for(int xo = tl.x, xi = max(0, -p.x);
        xo < frame->size.x && xi < img.size.x; ++xo, ++xi)
      frame->data[yo][xo] = img.data[yi][xi];
  if(blt)
    InvalidateRect(_win->hWnd, &get_rect(tl, sz), FALSE); }

void Panel::draw_selection(const ipoint& p0, const ipoint& pf, const bool blt){
  // Find selection to determine background color
  Cursor& c = cursor;
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
          text_to_frame(ipoint(x, p0.y)), blt); }
    return; }

  // Draw several lines
  for(int x = p0.x; x < text[p0.y].size(); ++x){
    color cb = (mark.y != -1 && in_selection(mark0, markf, ipoint(x, p0.y)))
        ? SELECT_COLOR : bkgd;
    draw_char(fonts[text_scale][cb][text_color[p0.y][x]][text[p0.y][x]],
        text_to_frame(ipoint(x, p0.y)), blt); }
  for(int y = p0.y + 1; y <= pf.y - 1; ++y)
    for(int x = 0; x < text[y].size(); ++x){
      color cb = (mark.y != -1 && in_selection(mark0, markf, ipoint(x, y)))
          ? SELECT_COLOR : bkgd;
      draw_char(fonts[text_scale][cb][text_color[y][x]][text[y][x]],
          text_to_frame(ipoint(x, y)), blt); }
  for(int x = 0; x <= min((int)text[pf.y].size() - 1, pf.x); ++x){
    color cb = (mark.y != -1 && in_selection(mark0, markf, ipoint(x, pf.y)))
        ? SELECT_COLOR : bkgd;
    draw_char(fonts[text_scale][cb][text_color[pf.y][x]][text[pf.y][x]],
        text_to_frame(ipoint(x, pf.y)), blt); }

  // Draw empty space
  for(int y = p0.y; y < pf.y; ++y)
    for(int x = (int)text[y].size(); x <= PANEL_CHARS; ++x){
      color cb = (mark.y != -1 && in_selection(mark0, markf, ipoint(x, y)))
          ? SELECT_COLOR : bkgd;
      draw_char(fonts[text_scale][cb][COLOR_CODE][' '],
          text_to_frame(ipoint(x, y)), blt); } }

void Panel::draw_divider(const bool blt){
  for(int y = pos.y; y < pos.y + size.y; ++y)
    for(int x = pos.x + size.x; x < pos.x + size.x + VERTICAL_DIVIDE; ++x)
      frame->data[y][x] = DIVIDER_COLOR;
  //! draw scroll marker
  if(blt)
    InvalidateRect(_win->hWnd, &get_rect(ipoint(pos.x + size.x, pos.y),
        ipoint(VERTICAL_DIVIDE, size.y)), FALSE); }

void Panel::draw_cursor_pos(const bool blt){
  int n = file_bar_text().size();
  str pos_text = "(" + to_string(cursor.pos.y + 1) + ","
      + to_string(cursor.pos.x + 1) + ")     ";
  color cbkgd = focus ? FOCUS_FILE_BAR_COLOR : UNFOCUS_FILE_BAR_COLOR;
  for(int x = 0; x < pos_text.size(); ++x)
    draw_char(fonts[1.0][cbkgd][BAR_TEXT_COLOR][pos_text[x]],
        ipoint(pos.x + (x + n) * char_width, pos.y + size.y), blt); }

void Panel::draw_file_bar(const bool blt){
  // Draw bar
  color cbkgd = focus ? FOCUS_FILE_BAR_COLOR : UNFOCUS_FILE_BAR_COLOR;
  for(int y = pos.y + size.y; y < pos.y + size.y + LINE_HEIGHT_SCALE_1; ++y)
    for(int x = pos.x; x < pos.x + size.x + VERTICAL_DIVIDE; ++x)
      frame->data[y][x] = cbkgd;
  // Draw text
  str bar_text = file_bar_text();
  for(int x = 0; x < bar_text.size(); ++x)
    draw_char(fonts[1.0][cbkgd][BAR_TEXT_COLOR][bar_text[x]],
        ipoint(pos.x + x * char_width, pos.y + size.y), false);
  draw_cursor_pos(false);
  // Blt to window
  if(blt)
    InvalidateRect(_win->hWnd, &get_rect(ipoint(pos.x, pos.y + size.y),
        ipoint(size.x, LINE_HEIGHT_SCALE_1)), FALSE); }

// Handled by panel.update when in focus
void Cursor::draw(const ipoint& win_pos, const bool blt){
  updated = false;
  for(int y = win_pos.y; y < win_pos.y + size.y; ++y){
    frame->data[y][win_pos.x] = CURSOR_COLOR;
    frame->data[y][win_pos.x + size.x - 1] = CURSOR_COLOR; }
  for(int x = win_pos.x; x < win_pos.x + size.x; ++x){
    frame->data[win_pos.y][x] = CURSOR_COLOR;
    frame->data[win_pos.y + size.y - 1][x] = CURSOR_COLOR; }
  if(blt)
    InvalidateRect(_win->hWnd, &get_rect(win_pos, size), FALSE); }

#endif

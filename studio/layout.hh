// STUDIO PANEL LAYOUT

#ifndef LAYOUT_HH
#define LAYOUT_HH

// Resize panels in response to a window resize
//! probably needs to consider offsets & frame_size
void Studio::resize(){
  // if(nsize.x == size.x && nsize.y == size.y){
  //   win_pos = npos;
  //   return; }
  // frame.set_size(npos);
  // for(int y = 0; y < npos.y; ++y)
  //   for(int x = 0; x < npos.x; ++x)
  //     frame.data[y][x] = BKGD_COLOR;
  // double xr = (double)nsize.x / size.x;
  // double yr = (double)nsize.y / size.y;
  // for(Panel& p : panels){
  //   p.pos.x = (int)round(p.pos.x * xr);
  //   p.pos.y = (int)round(p.pos.y * yr);
  //   p.size.x = (int)round(xr * p.size.x);
  //   p.size.y = (int)round(yr * p.size.y); }
  // win_pos = npos, size = nsize;
}

// Switch focus to a different panel
void Studio::switch_panel(Panel* p){
  Panel& p0 = *focus;
  Cursor& c0 = p0.cursor;
  c0.blink = false;
  c0.fill = p0.bkgd;
  char ch = (c0.pos.x == p0.text[c0.pos.y].size())
      ? ' ' : p0.text[c0.pos.y][c0.pos.x];
  color tc = (ch == ' ') ? COLOR_CODE : p0.text_color[c0.pos.y][c0.pos.x];
  p0.draw_char(Panel::fonts[p0.text_scale][c0.fill][tc][ch],
      p0.text_to_frame(c0.pos), true);
  c0.draw(p0.text_to_frame(c0.pos), true);
  p0.focus = false;
  if(&p0 != &cmd_panel && p != &cmd_panel)
    p0.draw_file_bar(true);
  focus = p;
  p->focus = true;
  if(&p0 != &cmd_panel && p != &cmd_panel)
    p->draw_file_bar(true); }

// Move focus one panel left
void Studio::left_panel(){
  int i;
  for(i = 0; i < panels.size(); ++i)
    if(focus == &panels[i]) break;
  switch_panel(&panels[(i == 0) ? panels.size() - 1 : i - 1]); }

// Move focus one panel right
void Studio::right_panel(){
  int i;
  for(i = 0; i < panels.size(); ++i)
    if(focus == &panels[i]) break;
  switch_panel(&panels[(i == panels.size() - 1) ? 0 : i + 1]); }

// Split a panel into two, top and bottom
void Studio::split_horizontal(){
  Panel& p = *focus;
  int h1 = (int)ceil((double)(p.size.y + LINE_HEIGHT_SCALE_1) / 2.0);
  int h2 = (int)floor((double)(p.size.y + LINE_HEIGHT_SCALE_1) / 2.0);
  p.size.y = h1 - LINE_HEIGHT_SCALE_1;
  int i;
  for(i = 0; i < panels.size(); ++i)
    if(focus == &panels[i]) break;
  if(i + 1 == panels.size())
    panels.pb(p);
  else
    panels.insert(panels.begin() + i + 1, p);
  Panel& p2 = panels[i];
  Panel& p3 = panels[i + 1];
  p3.pos = ipoint(p2.pos.x, p2.pos.y + p2.size.y + LINE_HEIGHT_SCALE_1);
  p3.size.y = h2 - LINE_HEIGHT_SCALE_1;
  p3.focus = false;
  focus = &p2;
  p2.draw_divider(true);
  p3.draw(true);
  switch_panel(&p3); }
// Split a panel into two, left and right
void Studio::split_vertical(){
  Panel& p = *focus;
  int w1 = (int)ceil((double)(p.size.x + VERTICAL_DIVIDE) / 2.0);
  int w2 = (int)floor((double)(p.size.x + VERTICAL_DIVIDE) / 2.0);
  p.size.x = w1 - VERTICAL_DIVIDE;
  int i;
  for(i = 0; i < panels.size(); ++i)
    if(focus == &panels[i]) break;
  if(i + 1 == panels.size())
    panels.pb(p);
  else
    panels.insert(panels.begin() + i + 1, p);
  Panel& p2 = panels[i];
  Panel& p3 = panels[i + 1];
  p3.pos = ipoint(p2.pos.x + p2.size.x + VERTICAL_DIVIDE, p2.pos.y);
  p3.size.x = w2 - VERTICAL_DIVIDE;
  p3.focus = false;
  focus = &p2;
  p2.draw_divider(true);
  p3.draw(true);
  switch_panel(&p3); }

// Close the focus panel and expand others to fill the gap
//! Currently can only close a panel where just one is needed to fill gap
void Studio::close_panel(){
  Panel& pdel = *focus;
  // Find panel to expand
  Panel* xpd = NULL;
  for(Panel& p : panels){
    if(&p == &pdel) continue;
    xpd = &p;

    // Is expanding panel above closing panel
    if(p.size.x == pdel.size.x && p.pos.x == pdel.pos.x
        && p.pos.y + p.size.y + LINE_HEIGHT_SCALE_1 == pdel.pos.y){
      p.size.y += pdel.size.y + LINE_HEIGHT_SCALE_1;
      break;

    // Is expanding panel below closing panel
    }else if(p.size.x == pdel.size.x && p.pos.x == pdel.pos.x
        && p.pos.y == pdel.pos.y + pdel.size.y + LINE_HEIGHT_SCALE_1){
      p.size.y += pdel.size.y + LINE_HEIGHT_SCALE_1;
      p.pos.y = pdel.pos.y;
      break;

    // Is expanding panel left of closing panel
    }else if(p.size.y == pdel.size.y && p.pos.y == pdel.pos.y
        && p.pos.x + p.size.x + VERTICAL_DIVIDE == pdel.pos.x){
      p.size.x += pdel.size.x + VERTICAL_DIVIDE;
      break;

    // Is expanding panel right of closing panel
    }else if(p.size.y == pdel.size.y && p.pos.y == pdel.pos.y
        && p.pos.x == pdel.pos.x + pdel.size.x + VERTICAL_DIVIDE){
      p.size.x += pdel.size.x + VERTICAL_DIVIDE;
      p.pos.x = pdel.pos.x;
      break;
    }else
      xpd = NULL; }
  if(xpd == NULL) return;

  // Delete original panel and draw
  int i;
  for(i = 0; i < panels.size(); ++i)
    if(&pdel == &panels[i]) break;
  switch_panel(xpd);
  int j;
  for(j = 0; j < panels.size(); ++j)
    if(xpd == &panels[j]) break;
  panels.erase(panels.begin() + i);
  if(i < j)
    --focus;
  draw_all(); }

#endif

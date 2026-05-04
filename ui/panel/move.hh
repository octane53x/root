// MOVE CURSOR

#ifndef PANEL_MOVE_HH
#define PANEL_MOVE_HH

void Editor::move_cursor(const Dir d){
  Cursor& c = cursor;
  c.blink = true;
  c.fill = CURSOR_COLOR;
  c.updated = true;
  c.last_update = clock();
  ipoint cp = c.pos;

  // Move cursor
  switch(d){
  case UP:
    if(c.pos.y == 0){
      if(c.pos.x > 0)
        c.pos.x = 0;
      break; }
    --c.pos.y;
    if(c.pos.x > text[c.pos.y].size())
      c.pos.x = (int)text[c.pos.y].size();
    if(c.pos.y < top_line)
      scroll(UP);
    break;

  case LEFT:
    if(c.pos.x == 0){
      if(c.pos.y == 0) break;
      --c.pos.y;
      c.pos.x = (int)text[c.pos.y].size();
      if(c.pos.y < top_line)
        scroll(UP);
    }else
      --c.pos.x;
    break;

  case DOWN:
    if(c.pos.y == text.size() - 1){
      if(c.pos.x < text[c.pos.y].size())
        c.pos.x = (int)text[c.pos.y].size();
      break; }
    ++c.pos.y;
    if(c.pos.x > text[c.pos.y].size())
      c.pos.x = (int)text[c.pos.y].size();
    if(c.pos.y - top_line >= size.y / line_height - 1)
      scroll(DOWN);
    break;

  case RIGHT:
    if(c.pos.x == text[c.pos.y].size()){
      if(c.pos.y == text.size() - 1) break;
      ++c.pos.y;
      c.pos.x = 0;
      if(c.pos.y - top_line >= size.y / line_height - 1)
        scroll(DOWN);
    }else
      ++c.pos.x;
    break;
  default:
    err("move_cursor", "impossible direction"); }

  // Draw whole lines if mark is set
  if(mark.y != -1 && (c.pos.y != cp.y
      || (c.pos.y == cp.y && abs(c.pos.x - cp.x) > 1))){
    ipoint p0 = (c.pos.y < cp.y || (c.pos.y == cp.y && c.pos.x < cp.x))
        ? c.pos : cp;
    ipoint pf = (c.pos == p0) ? cp : c.pos;
    draw_selection(p0, pf, true); }

  // Draw char at previous position
  color cb = bkgd;
  if(mark.y != -1){
    ipoint p0 = (c.pos.y < mark.y || (c.pos.y == mark.y && c.pos.x < mark.x))
        ? c.pos : mark;
    ipoint pf = (c.pos == p0) ? mark : c.pos;
    if(in_selection(p0, ipoint(pf.x - 1, pf.y), cp))
      cb = SELECT_COLOR; }
  char ch = (cp.x == text[cp.y].size()) ? ' ' : text[cp.y][cp.x];
  color ct = (ch == ' ') ? (cmd ? BAR_TEXT_COLOR : COLOR_CODE)
      : text_color[cp.y][cp.x];
  draw_char(fonts[text_scale][cb][ct][ch], text_to_frame(cp), true);

  // Draw character and cursor
  ch = (c.pos.x == text[c.pos.y].size()) ? ' ' : text[c.pos.y][c.pos.x];
  draw_char(fonts[text_scale][c.fill][BAR_TEXT_COLOR][ch],
      text_to_frame(c.pos), true);

  // Update file bar
  if(!cmd)
    draw_cursor_pos(true); }

void Studio::ctrl_move(const Dir d){
  Panel& p = *focus;
  Cursor& c = p.cursor;
  if(d == UP){
    p.move_cursor(UP);
    while(c.pos.y > 0 && p.text[c.pos.y] != "")
      p.move_cursor(UP);
    while(c.pos.x > 0)
      p.move_cursor(LEFT);
  }else if(d == LEFT){
    p.move_cursor(LEFT);
    while(!(c.pos.y == 0 && c.pos.x == 0)
        && !name_or_val(p.text[c.pos.y][c.pos.x]))
      p.move_cursor(LEFT);
    while(!(c.pos.y == 0 && c.pos.x == 0)
        && name_or_val(p.text[c.pos.y][c.pos.x]))
      p.move_cursor(LEFT);
    if(!(c.pos.y == 0 && c.pos.x == 0))
      p.move_cursor(RIGHT);
  }else if(d == DOWN){
    p.move_cursor(DOWN);
    while(c.pos.y < p.text.size() - 1 && p.text[c.pos.y] != "")
      p.move_cursor(DOWN);
    while(c.pos.x < p.text[c.pos.y].size())
      p.move_cursor(RIGHT);
  }else if(d == RIGHT){
    while(!(c.pos.y == p.text.size() - 1 && c.pos.x == p.text[c.pos.y].size())
        && !name_or_val(p.text[c.pos.y][c.pos.x]))
      p.move_cursor(RIGHT);
    while(!(c.pos.y == p.text.size() - 1 && c.pos.x == p.text[c.pos.y].size())
        && name_or_val(p.text[c.pos.y][c.pos.x]))
      p.move_cursor(RIGHT); } }

void Studio::move_max(const Dir d){
  Panel& p = *focus;
  Cursor& c = p.cursor;
  if(d == UP){
    c.pos = ipoint(0, 0);
    p.top_line = 0;
    p.draw(true);
  }else if(d == LEFT){
    while(c.pos.x > 0)
      p.move_cursor(LEFT);
  }else if(d == DOWN){
    c.pos = ipoint((int)p.text.back().size(), (int)p.text.size() - 1);
    p.top_line = max(0, (int)p.text.size() - (p.size.y / p.line_height));
    p.draw(true);
  }else if(d == RIGHT){
    while(c.pos.x < p.text[c.pos.y].size())
      p.move_cursor(RIGHT); } }

void InputPanel::move_up(){

}

void InputPanel::move_down(){

}

void InputPanel::move_left(){

}

void InputPanel::move_right(){

}

void InputPanel::move_up_block(){

}

void InputPanel::move_down_block(){

}

void InputPanel::move_left_word(){

}

void InputPanel::move_right_word(){

}

void InputPanel::move_up_max(){

}

void InputPanel::move_down_max(){

}

void InputPanel::move_left_max(){

}

void InputPanel::move_right_max(){

}

#endif

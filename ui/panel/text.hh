// PANEL TEXT OPERATIONS

#ifndef PANEL_TEXT_HH
#define PANEL_TEXT_HH

void TextPanel::insert_text(const vec<str>& ins, const ipoint& p){
  // Store operation for undo
  Op op;
  op.ins = true;
  op.pos = p;
  op.text = ins;
  opstack.pb(op);

  // Modify text
  str tail = text[p.y].substr(p.x);
  text[p.y] = text[p.y].substr(0, p.x) + ins[0];
  text.insert(text.begin() + p.y + 1, ins.begin() + 1, ins.end());
  text[p.y + ins.size() - 1] += tail;
  text_color.insert(text_color.begin() + p.y + 1, ins.size() - 1, vec<color>());
  highlight_text(p.y, p.y + ins.size() - 1);

  // Determine selection
  Cursor& c = cursor;
  ipoint mark0, markf;
  if(mark.y != -1){
    mark0 = (c.pos.y < mark.y || (c.pos.y == mark.y && c.pos.x < mark.x))
        ? c.pos : mark;
    markf = (mark0 == c.pos) ? mark : c.pos; }

  // Draw single character at end of line
  // if(ins.size() == 1 && ins[0].size() == 1 && p.x == text[p.y].size() - 1){
  //   color cb = (mark.y != -1 && in_selection(mark0, markf, p))
  //       ? SELECT_COLOR : bkgd;
  //   draw_char(fonts[text_scale][cb][text_color[p.y][p.x]][ins[0][0]],
  //       text_to_frame(p), true);
  //   return; }

  // Draw single line without adding new lines
  if(ins.size() == 1){
    for(int x = 0; x < text[p.y].size(); ++x){
      color cb = (mark.y != -1 && in_selection(mark0, markf, ipoint(x, p.y)))
          ? SELECT_COLOR : bkgd;
      draw_char(fonts[text_scale][cb][text_color[p.y][x]][text[p.y][x]],
          text_to_frame(ipoint(x, p.y)), true); }
    if(saved){
      saved = false;
      draw_file_bar(true); }
    return; }

  // Redraw panel if lines moved
  saved = false;
  draw(true); }

void TextPanel::remove_text(const ipoint& p0, const ipoint& pf){
  if(p0.y == text.size() - 1 && p0.x == text[p0.y].size()) return;
  bool endline = (p0.y == pf.y && p0.x == text[p0.y].size());

  // Adjust endpoint
  ipoint pf2 = pf;
  if(pf.x == -1){
    --pf2.y;
    pf2.x = (int)text[pf2.y].size(); }

  // Store operation for undo
  Op op;
  op.ins = false;
  op.pos = p0;
  op.text.pb("");
  ipoint pt = p0;
  while(1){
    if(pt.x == text[pt.y].size()){
      op.text.pb("");
      if(pt == pf2) break;
      ++pt.y;
      pt.x = 0;
      continue; }
    op.text.back() += str(1, text[pt.y][pt.x]);
    if(pt == pf2) break;
    ++pt.x; }
  opstack.pb(op);

  // Determine selection
  Cursor& c = cursor;
  ipoint mark0, markf;
  if(mark.y != -1){
    mark0 = (c.pos.y < mark.y || (c.pos.y == mark.y && c.pos.x < mark.x))
        ? c.pos : mark;
    markf = (mark0 == c.pos) ? mark : c.pos; }

  // Draw over end of line
  if(p0.y == pf.y && !endline){
    color tc = cmd ? BAR_TEXT_COLOR : COLOR_CODE;
    for(int x = (int)text[p0.y].size() - (pf.x - p0.x + 1);
        x <= text[p0.y].size(); ++x){
      color cb = (mark.y != -1 && in_selection(mark0, markf, ipoint(x, p0.y)))
          ? SELECT_COLOR : bkgd;
      draw_char(fonts[text_scale][cb][tc][' '],
          text_to_frame(ipoint(x, p0.y)), true); } }

  // Remove from text
  str line = text[p0.y].substr(0, p0.x);
  if(pf2.x == text[pf2.y].size()){
    if(pf2.y + 1 < text.size() && text[pf2.y + 1] != "")
      line += text[pf2.y + 1];
    text.erase(text.begin() + p0.y + 1, text.begin() + pf2.y + 2);
    text_color.erase(text_color.begin() + p0.y + 1,
        text_color.begin() + pf2.y + 2);
  }else{
    line += text[pf2.y].substr(pf2.x + 1);
    text.erase(text.begin() + p0.y + 1, text.begin() + pf2.y + 1);
    text_color.erase(text_color.begin() + p0.y + 1,
        text_color.begin() + pf2.y + 1); }
  text[p0.y] = line;
  highlight_text(p0.y, p0.y);

  // Draw chars after deletion
  if(p0.y == pf.y && !endline){
    for(int x = p0.x; x < text[p0.y].size(); ++x){
      color cb = (mark.y != -1 && in_selection(mark0, markf, ipoint(x, p0.y)))
          ? SELECT_COLOR : bkgd;
      draw_char(fonts[text_scale][cb][text_color[p0.y][x]][text[p0.y][x]],
          text_to_frame(ipoint(x, p0.y)), true); }
    return; }

  // Redraw panel if lines moved
  saved = false;
  draw(true); }

void InputPanel::newline(){
  Panel& p = *focus;
  Cursor& c = p.cursor;
  p.delete_selection();
  p.insert_text(vec<str>({"", ""}), c.pos);
  p.move_cursor(RIGHT);
  int spaces = 0;
  for(; spaces < p.text[c.pos.y - 1].size(); ++spaces)
    if(p.text[c.pos.y - 1][spaces] != ' ') break;
  for(int i = 0; i < spaces; ++i){
    p.insert_text(vec<str>({" "}), c.pos);
    p.move_cursor(RIGHT); } }

void InputPanel::del(){
  Panel& p = *focus;
  Cursor& c = p.cursor;
  if(p.mark.y == -1)
    p.remove_text(c.pos, c.pos);
  else
    p.delete_selection(); }

void InputPanel::del_space(){
  Panel& p = *focus;
  Cursor& c = p.cursor;
  while(c.pos.x > 0 && p.text[c.pos.y][c.pos.x - 1] == ' '){
    p.move_cursor(LEFT);
    p.remove_text(c.pos, c.pos); }
  while(c.pos.x < p.text[c.pos.y].size() && p.text[c.pos.y][c.pos.x] == ' ')
    p.remove_text(c.pos, c.pos); }

void InputPanel::del_back(){
  Panel& p = *focus;
  Cursor& c = p.cursor;
  if(p.mark.y == -1){
    if(&p == &cmd_panel){
      if(p.text[0].find("Open: ") == 0 && c.pos.x == 6) return;
      if(p.text[0].find("Save: ") == 0 && c.pos.x == 6) return;
      if(p.text[0].find("Goto: ") == 0 && c.pos.x == 6) return; }
    p.move_cursor(LEFT);
    p.remove_text(c.pos, c.pos);
  }else
    p.delete_selection(); }

void InputPanel::del_back_word(){
  Panel& p = *focus;
  Cursor& c = p.cursor;
  if(c.pos.x > 1 && p.text[c.pos.y][c.pos.x - 1] == ' '
      && p.text[c.pos.y][c.pos.x - 2] == ' '){
    p.remove_text(ipoint(c.pos.x - 2, c.pos.y), ipoint(c.pos.x - 1, c.pos.y));
    p.move_cursor(LEFT);
    p.move_cursor(LEFT);
    return; }
  int y0 = c.pos.y, x0 = c.pos.x;
  while(!(c.pos.y == 0 && c.pos.x == 0)
      && !name_or_val(p.text[c.pos.y][c.pos.x]))
    p.move_cursor(LEFT);
  while(!(c.pos.y == 0 && c.pos.x == 0)
      && name_or_val(p.text[c.pos.y][c.pos.x])){
    p.move_cursor(LEFT);
    if(c.pos.x == p.text[c.pos.y].size())
      break; }
  if(!(c.pos.y == 0 && c.pos.x == 0))
    p.move_cursor(RIGHT);
  p.remove_text(c.pos, ipoint(x0 - 1, y0)); }

void InputPanel::del_selection(){
  if(mark.y == -1) return;
  Cursor& c = cursor;
  ipoint p0 = (c.pos.y < mark.y || (c.pos.y == mark.y && c.pos.x < mark.x))
      ? c.pos : mark;
  ipoint pf = (c.pos == p0) ? mark : c.pos;
  remove_text(p0, ipoint(pf.x - 1, pf.y));
  if(c.pos == pf)
    c.pos = p0;
  mark = ipoint(-1, -1);
  draw(true); }

void InputPanel::indent(){
  Panel& p = *focus;
  Cursor& c = p.cursor;
  if(c.pos.y > 0){
    int spaces = 0;
    for(; spaces < p.text[c.pos.y - 1].size(); ++spaces)
      if(p.text[c.pos.y - 1][spaces] != ' ') break;
    if(c.pos.x < spaces){
      int x = spaces - c.pos.x;
      for(int i = 0; i < x; ++i){
        p.insert_text(vec<str>({" "}), c.pos);
        p.move_cursor(RIGHT); }
      return; } }
  if(c.pos.x & 1){
    p.insert_text(vec<str>({" "}), c.pos);
    p.move_cursor(RIGHT);
  }else{
    p.insert_text(vec<str>({"  "}), c.pos);
    p.move_cursor(RIGHT);
    p.move_cursor(RIGHT); } }

void InputPanel::indent_selection(){
  Panel& p = *focus;
  Cursor& c = p.cursor;
  if(p.mark.y == -1) return;
  for(int y = min(p.mark.y, c.pos.y); y <= max(p.mark.y, c.pos.y); ++y)
    if(p.text[y] != "")
      p.insert_text(vec<str>({"  "}), ipoint(0, y)); }

void InputPanel::unindent_selection(){
  Panel& p = *focus;
  Cursor& c = p.cursor;
  if(p.mark.y == -1) return;
  for(int y = min(p.mark.y, c.pos.y); y <= max(p.mark.y, c.pos.y); ++y)
    if(p.text[y].size() > 1 && p.text[y][0] == ' ' && p.text[y][1] == ' ')
      p.remove_text(ipoint(0, y), ipoint(1, y)); }

void InputPanel::undo(){
  Panel& p = *focus;
  Cursor& c = p.cursor;
  if(p.opstack.empty()) return;
  Panel::Op op = p.opstack.back();
  p.opstack.popb();
  if(op.ins){
    ipoint pf = op.pos;
    if(op.text.size() == 1){
      pf.x += (int)op.text[0].size() - 1;
    }else{
      pf.y += (int)op.text.size() - 1;
      pf.x = (int)op.text.back().size() - 1; }
    p.remove_text(op.pos, pf);
    c.pos = op.pos;
  }else{
    p.insert_text(op.text, op.pos);
    c.pos = op.pos;
    if(op.text.size() == 1){
      c.pos.x += (int)op.text[0].size();
    }else{
      c.pos.x = (int)op.text.back().size();
      c.pos.y += (int)op.text.size() - 1; } }
  p.opstack.popb(); }

#endif

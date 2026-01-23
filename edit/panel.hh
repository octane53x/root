// EDITOR PANEL

#ifndef PANEL_HH
#define PANEL_HH

#include "cursor.hh"

struct Panel {

  // Used for undo
  struct Op {
    bool ins; // False if remove
    ipoint pos;
    vec<str> text; };

  bool updated, cmd, saved, focus, split_ready;
  int line_height, char_width, scroll_lines, top_line;
  double text_scale;
  clock_t last_update;
  color bkgd;
  ipoint pos, size, mark;
  str dir, file;
  vec<str> text;
  vec<vec<color> > text_color;
  vec<Op> opstack;
  FileType file_type;
  Cursor cursor;

  // Key 1: Scale, Key 2: Bkgd color, Key 3: Text color, Key 4: Character
  static umap<double, umap<color, umap<color, font> > > fonts;
  // Editor window frame
  static image* frame;

  Panel();

  ipoint text_to_frame(const ipoint& p) const;
  str file_bar_text() const;

  void init();
  void update(const double ms);

  // Defined in draw.hh
  void draw(const bool blt);
  void draw_char(const image& img, const ipoint& p,
      const bool blt);
  void draw_selection(const ipoint& p0, const ipoint& pf,
      const bool blt);
  void draw_divider(const bool blt);
  void draw_cursor_pos(const bool blt);
  void draw_file_bar(const bool blt);

  void resize(const ipoint& _pos, const ipoint& _size);
  void insert_text(const vec<str>& ins, const ipoint& p);
  void remove_text(const ipoint& p0, const ipoint& pf);
  void delete_selection();
  void clean();
  void scroll(const Dir d);
  void move_cursor(const Dir d);
  void set_file_type();
  // Defined in highlight.hh
  void highlight_text(); };

umap<double, umap<color, umap<color, font> > > Panel::fonts;
image* Panel::frame;

Panel::Panel():
    size(0, 0), pos(0, 0) {}

ipoint Panel::text_to_frame(const ipoint& p) const {
  return ipoint(p.x * char_width + pos.x,
      (p.y - top_line) * line_height + pos.y); }

str Panel::file_bar_text() const {
  str bar_text = str(saved ? "-----" : "*****") + "     ";
  if(dir.find("/root/") != str::npos)
    bar_text += dir.substr(dir.find("/root/") + 6) + file + "     ";
  return bar_text; }

// Set size and pos prior
void Panel::init(){
  cmd = split_ready = false;
  saved = focus = true;
  line_height = LINE_HEIGHT_SCALE_1;
  char_width = CHAR_WIDTH_SCALE_1;
  scroll_lines = size.y / line_height / 2;
  top_line = 0;
  text_scale = 1.0;
  bkgd = BKGD_COLOR;
  mark = ipoint(-1, -1);
  file = "";
  text.pb("");
  text_color.pb(vec<color>());
  dir = clean_path(current_path().string());
  file_type = NO_FILE_TYPE;
  cursor.init(); }

void Panel::update(const double ms){
  Cursor& c = cursor;
  if(!focus) return;
  c.update(ms);
  if(!c.updated) return;
  c.updated = false;
  // Draw character or cursor
  char ch = (c.pos.x == text[c.pos.y].size())
      ? ' ' : text[c.pos.y][c.pos.x];
  color ct = (c.fill == CURSOR_COLOR || cmd) ? BAR_TEXT_COLOR
      : ((ch == ' ') ? COLOR_CODE : text_color[c.pos.y][c.pos.x]);
  color cb = (c.fill == bkgd && mark.y != -1 && (c.pos.y < mark.y
      || (c.pos.y == mark.y && c.pos.x < mark.x))) ? SELECT_COLOR : c.fill;
  draw_char(fonts[text_scale][cb][ct][ch], text_to_frame(c.pos), true);
  updated = true;
  last_update = clock(); }

void Panel::insert_text(const vec<str>& ins, const ipoint& p){
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
  highlight_text();

  // Determine selection
  Cursor& c = cursor;
  ipoint mark0, markf;
  if(mark.y != -1){
    mark0 = (c.pos.y < mark.y || (c.pos.y == mark.y && c.pos.x < mark.x))
        ? c.pos : mark;
    markf = (mark0 == c.pos) ? mark : c.pos; }

  // Draw single character at end of line
  if(ins.size() == 1 && ins[0].size() == 1 && p.x == text[p.y].size() - 1){
    color cb = (mark.y != -1 && in_selection(mark0, markf, p))
        ? SELECT_COLOR : bkgd;
    draw_char(fonts[text_scale][cb][text_color[p.y][p.x]][ins[0][0]],
        text_to_frame(p), true);
    return; }

  // Draw single line without adding new lines
  if(ins.size() == 1){
    for(int x = p.x; x < text[p.y].size(); ++x){
      color cb = (mark.y != -1 && in_selection(mark0, markf, ipoint(x, p.y)))
          ? SELECT_COLOR : bkgd;
      draw_char(fonts[text_scale][cb][text_color[p.y][x]][text[p.y][x]],
          text_to_frame(ipoint(x, p.y)), true); }
    return; }

  // Redraw panel if lines moved
  saved = false;
  draw(true); }

void Panel::remove_text(const ipoint& p0, const ipoint& pf){
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
  }else{
    line += text[pf2.y].substr(pf2.x + 1);
    text.erase(text.begin() + p0.y + 1, text.begin() + pf2.y + 1); }
  text[p0.y] = line;
  highlight_text();

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

void Panel::delete_selection(){
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

void Panel::clean(){
  Cursor& c = cursor;
  // Delete trailing whitespace
  for(int y = 0; y < text.size(); ++y){
    int n = 0;
    for(int x = text[y].size() - 1; x >= 0; --x){
      if(text[y][x] == ' ') ++n;
      else break; }
    text[y] = text[y].substr(0, text[y].size() - n); }
  // Reset cursor
  while(c.pos.x > text[c.pos.y].size())
    --c.pos.x; }

void Panel::scroll(const Dir d){
  int lines = min((int)text.size() - top_line - 1, scroll_lines);
  if(d == UP)
    lines = min(lines, top_line);
  top_line += (d == DOWN) ? lines : -lines;
  draw(true); }

void Panel::move_cursor(const Dir d){
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
  color ct = (ch == ' ') ? COLOR_CODE : text_color[cp.y][cp.x];
  draw_char(fonts[text_scale][cb][ct][ch], text_to_frame(cp), true);

  // Draw character and cursor
  ch = (c.pos.x == text[c.pos.y].size()) ? ' ' : text[c.pos.y][c.pos.x];
  draw_char(fonts[text_scale][c.fill][BAR_TEXT_COLOR][ch],
      text_to_frame(c.pos), true);

  // Update file bar
  if(!cmd)
    draw_cursor_pos(true); }

void Panel::set_file_type(){
  if(ends_with(file, ".cc") || ends_with(file, ".hh"))
    file_type = CPP;
  else if(ends_with(file, ".py"))
    file_type = PYTH;
  else if(ends_with(file, ".crab"))
    file_type = CRAB;
  else
    file_type = NO_FILE_TYPE; }

#endif

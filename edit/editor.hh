// EDITOR

// Depends on libpng/zlib - loaded in Visual Studio

#ifndef EDITOR_HH
#define EDITOR_HH

#include <png.h>
#include "window.hh"
#include "panel.hh"

// Text editor application
struct Editor : virtual window {

  bool shift, ctrl, alt;
  vec<str> clipboard;
  font font_base;
  Panel cmd, info, *focus, *prev_panel;
  vec<Panel> panels;

  KeyProc keyproc;
  CmdProc cmdproc;

  void init_members(const HINSTANCE wp1, const int wp2);

  virtual void init();
  virtual void run();
  virtual void update(const double ms);
  virtual void draw();
  virtual void resize(const point& pos, const int w, const int h);

  void load_font();
  void color_font(const double scale);
  image color_char(const image& img, const color& ctext, const color& cbkgd);
  void clip();
  void split_horizontal();
  void split_vertical();
  void close_panel(); };

void Editor::init_members(const HINSTANCE wp1, const int wp2){
  win_param_1 = wp1, win_param_2 = wp2; }

void Editor::init(){
  system::init();
  _win = this;
  updated = true;
  shift = ctrl = alt = false;

  // Window init
  RECT rect;
  SystemParametersInfo(SPI_GETWORKAREA, 0, &rect, 0);
  size = ipoint(2 * (PANEL_CHARS * (int)round(INIT_TEXT_SCALE
      * CHAR_WIDTH_SCALE_1) + VERTICAL_DIVIDE + PANEL_OFFSET),
      rect.bottom - rect.top);
  win_pos = ipoint(rect.right - size.x, 0);
  size.x += WINDOW_OFFSET;
  size.y += WINDOW_OFFSET;

  // Initial panel
  panels.pb(Panel());
  Panel& p = panels.back();
  p.size = ipoint(width - VERTICAL_DIVIDE + WIDTH_OFFSET,
      height - p.line_height * 2 + HEIGHT_OFFSET);
  p.init();
  focus = &p;

  // Command bar
  cmd.size = ipoint(size.x, LINE_HEIGHT_SCALE_1);
  cmd.init();
  cmd.pos = point(0, p.height + p.line_height);
  cmd.bkgd = cmd.cursor.bkgd = CMD_BAR_COLOR;
  cmd.cursor.blink = false;

  // Font
  load_font();
  color_font(1.0);

  // Display frame
  frame.set_size(size.x, size.y);
  frame.fill(BKGD_COLOR); }

void Editor::run(){
  system::run();
  for(Panel& p : panels){
    p.run();
    p.cursor.run(); }
  _win_init();
  _win_run(); }

void Editor::update(const double ms){
  system::update(ms);
  for(Panel& p : panels)
    if(p.refresh){
      p.update(ms);
      updated = true; }
  focus->cursor.update(ms);
  if(focus->cursor.updated)
    updated = true;
  if(updated)
    last_update = clock(); }

void Editor::draw(){
  Panel& p = *focus;
  Cursor& c = p.cursor;
  for(int y : p.refresh_lines){
    if(y < 0 || y > p.height / p.line_height)
      err("draw", "bad refresh line");
    int yl = y + p.top_line;

    // Determine selection
    int xstart = -1, xend = -1;
    if(p.ymark != -1){
      // Cursor before mark
      if(c.y < p.ymark || (c.y == p.ymark && c.x < p.xmark)){
        if(c.y < yl)
          xstart = 0;
        else if(c.y == yl)
          xstart = c.x;
        if(p.ymark > yl)
          xend = (int)p.text[yl].size();
        else if(p.ymark == yl)
          xend = p.xmark - 1;

      // Mark before cursor
      }else if(c.y > p.ymark || (c.y == p.ymark && c.x > p.xmark)){
        if(p.ymark < yl)
          xstart = 0;
        else if(p.ymark == yl)
          xstart = p.xmark;
        if(c.y > yl)
          xend = (int)p.text[yl].size();
        else if(c.y == yl)
          xend = c.x - 1; } }
    if(yl < p.text.size())
      assert(xend <= (int)p.text[yl].size(), "draw", "bad selection end point");

    // Line background
    if(xstart != 0 || xend == -1){
      polygon line;
      line.pos = point(p.pos.x, p.pos.y + y * p.line_height);
      int w = (xstart == -1 || xend == -1) ? p.width : xstart * p.char_width;
      line.set_box(w, p.line_height);
      line.fill = p.col;
      line.draw(&frame, p.view); }

    // Selection background
    if(xstart != -1 && xend != -1){
      polygon select;
      select.pos = point(p.pos.x + xstart * p.char_width,
          p.pos.y + y * p.line_height);
      int w = (xend == p.text[yl].size()) ? p.width - xstart * p.char_width
          : (xend - xstart + 1) * p.char_width;
      select.set_box(w, p.line_height);
      select.fill = SELECT_COLOR;
      select.draw(&frame, p.view); }

    // Line background
    if(xstart != -1 && xend != -1 && xend < p.text[yl].size()){
      polygon line;
      line.pos = point(p.pos.x + (xend + 1) * p.char_width,
          p.pos.y + y * p.line_height);
      line.set_box(p.width - (xend + 1) * p.char_width, p.line_height);
      line.fill = p.col;
      line.draw(&frame, p.view); }

    // Draw text
    if(yl >= p.text.size()) continue;
    if(&p != &cmd)
      highlight_text(yl);
    for(int x = 0; x < p.text[yl].size(); ++x){
      debug("Panel "+to_string((llu)&p)+" draw char "+str(1, p.text[yl][x])
          +" at "+point(p.pos.x + x * p.char_width,
          p.pos.y + y * p.line_height).to_str()+" \n  with color "
          +p.text_color[yl][x].to_str()+" size "
          +to_string(p.font[p.text[yl][x]].width)+","
          +to_string(p.font[p.text[yl][x]].height));
      draw_char(p.font[p.text[yl][x]],
          point(p.pos.x + x * p.char_width, p.pos.y + y * p.line_height),
          p.text_color[yl][x], p.col); } }
  p.refresh_lines.clear();

  if(c.xprev != c.x || c.yprev != c.y){
    // Clear last character
    color col = c.fill;
    c.fill =
        (p.ymark == -1 || ((c.y < c.yprev || (c.y == c.yprev && c.x < c.xprev))
        && (p.ymark < c.yprev || (p.ymark == c.yprev && p.xmark <= c.xprev)))
        || ((c.y > c.yprev || (c.y == c.yprev && c.x > c.xprev))
        && (p.ymark > c.yprev || (p.ymark == c.yprev && p.xmark > c.xprev))))
        ? p.col : SELECT_COLOR;
    c.pos = point(p.pos.x + c.xprev * p.char_width,
        p.pos.y + (c.yprev - p.top_line) * p.line_height);
    c.draw(&frame, p.view);
    c.pos = point(p.pos.x + c.x * p.char_width,
        p.pos.y + (c.y - p.top_line) * p.line_height);
    c.fill = col;

    // Draw last character
    if(c.yprev < p.text.size() && c.xprev < p.text[c.yprev].size())
      draw_char(p.font[p.text[c.yprev][c.xprev]],
          point(p.pos.x + c.xprev * p.char_width,
          p.pos.y + (c.yprev - p.top_line) * p.line_height),
          p.text_color[c.yprev][c.xprev], p.col);
    c.xprev = c.x, c.yprev = c.y; }

  // Clear cursor
  color col = c.fill;
  c.fill = (p.ymark == -1 || (c.y > p.ymark
      || (c.y == p.ymark && c.x >= p.xmark))) ? p.col : SELECT_COLOR;
  c.draw(&frame, p.view);
  c.fill = col;
  // Draw current character
  if(c.y < p.text.size() && c.x < p.text[c.y].size())
    draw_char(p.font[p.text[c.y][c.x]],
        point(p.pos.x + c.x * p.char_width,
        p.pos.y + (c.y - p.top_line) * p.line_height),
        p.text_color[c.y][c.x], p.col);
  // Draw cursor
  c.draw(&frame, p.view);

  // Draw vertical divider
  for(Panel& t : panels){
    if(!t.refresh_divider) continue;
    polygon div;
    div.pos = point(t.pos.x + t.width, t.pos.y);
    div.set_box(VERTICAL_DIVIDE, t.height);
    div.fill = UNFOCUS_FILE_BAR_COLOR;
    div.draw(&frame, t.view);
    t.refresh_divider = false; }

  // Draw file bars
  for(Panel& t : panels){
    if(!t.refresh_file_bar) continue;
    polygon bar;
    bar.pos = point(t.pos.x, t.pos.y + t.height);
    bar.set_box(t.width + VERTICAL_DIVIDE, default_line_height);
    bar.fill = (focus == &t) ? FOCUS_FILE_BAR_COLOR : UNFOCUS_FILE_BAR_COLOR;
    bar.draw(&frame, t.view);
    str bar_text = str(t.saved ? "-----" : "*****") + "     ";
    if(t.file.find("\\root\\") != str::npos)
      bar_text += t.file.substr(t.file.find("\\root\\") + 6) + "     ";
    bar_text += "(" + to_string(t.cursor.y + 1) + ","
        + to_string(t.cursor.x + 1) + ")";
    for(int x = 0; x < bar_text.size(); ++x)
      draw_char(default_font[bar_text[x]],
          point(bar.pos.x + x * default_char_width, bar.pos.y),
          FILE_BAR_TEXT_COLOR, bar.fill);
    t.refresh_file_bar = false; }

  // Hide cmd bar
  if(cmd.hide){
    assert(focus != &cmd, "draw", "cmd marked for refresh but still in focus");
    polygon line;
    line.pos = point(cmd.pos.x, cmd.pos.y);
    line.set_box(cmd.width, cmd.line_height);
    line.fill = BKGD_COLOR;
    line.draw(&frame, cmd.view);
    cmd.hide = false; } }

void Editor::resize(const point& pos, const int w, const int h){
  if(w == width && h == height){
    win_pos = pos;
    return; }
  frame.set_size(w, h);
  for(int y = 0; y < h; ++y)
    for(int x = 0; x < w; ++x)
      frame.set_pixel(x, y, BKGD_COLOR);
  double xr = (double)w / width;
  double yr = (double)h / height;
  for(Panel& p : panels){
    p.pos.x = round(p.pos.x * xr);
    p.pos.y = round(p.pos.y * yr);
    p.width = (int)round(xr * p.width);
    p.height = (int)round(yr * p.height); }
  win_pos = pos, width = w, height = h; }

void Editor::load_font(){
  // Load the font PNG with libpng
  FILE* fp;
  fopen_s(&fp, _FONT_LOC.c_str(), "rb");
  uchar header[8];
  fread(header, 1, 8, fp);
  png_sig_cmp(header, 0, 8);
  png_structp png_ptr = png_create_read_struct(
      PNG_LIBPNG_VER_STRING, NULL, NULL, NULL);
  png_infop info_ptr = png_create_info_struct(png_ptr);
  setjmp(png_jmpbuf(png_ptr));
  png_init_io(png_ptr, fp);
  png_set_sig_bytes(png_ptr, 8);
  png_read_info(png_ptr, info_ptr);
  int w = png_get_image_width(png_ptr, info_ptr);
  int h = png_get_image_height(png_ptr, info_ptr);
  png_byte color_type = png_get_color_type(png_ptr, info_ptr);
  png_byte bit_depth = png_get_bit_depth(png_ptr, info_ptr);
  if(bit_depth == 16)
    png_set_strip_16(png_ptr);
  if(color_type == PNG_COLOR_TYPE_PALETTE)
    png_set_palette_to_rgb(png_ptr);
  if(color_type == PNG_COLOR_TYPE_GRAY && bit_depth < 8)
    png_set_expand_gray_1_2_4_to_8(png_ptr);
  if(png_get_valid(png_ptr, info_ptr, PNG_INFO_tRNS))
    png_set_tRNS_to_alpha(png_ptr);
  png_read_update_info(png_ptr, info_ptr);
  int channels = png_get_channels(png_ptr, info_ptr);
  size_t rowbytes = png_get_rowbytes(png_ptr, info_ptr);
  unsigned char* image_data = new unsigned char[rowbytes * h];
  png_bytep* row_pointers = new png_bytep[h];
  for(int i = 0; i < h; ++i)
    row_pointers[i] = image_data + i * rowbytes;
  png_read_image(png_ptr, row_pointers);
  delete[] row_pointers;
  png_destroy_read_struct(&png_ptr, &info_ptr, NULL);
  fclose(fp);

  // Create the image from buffer
  image font_img;
  font_img.set_size(width, height);
  for(int y = 0; y < h; ++y)
    for(int x = 0; x < w; ++x)
      font_img.set_pixel(x, y, color(
          image_data[y * w * 4 + x * 4],
          image_data[y * w * 4 + x * 4 + 1],
          image_data[y * w * 4 + x * 4 + 2]));

  // Split characters from image
  for(int i = 0; i < _SYMBOLS.size(); ++i){
    image c(CHAR_WIDTH_SCALE_1, LINE_HEIGHT_SCALE_1);
    for(int xi = i * c.width, xo = 0; xo < c.width; ++xi, ++xo)
      for(int y = 0; y < c.height; ++y)
        c.set_pixel(xo, y, font_img.data[y][xi]);
    font_base[_SYMBOLS[i]] = c; } }

void Editor::color_font(const double scale){
  umap<color, umap<color, font> > cmbkgd;
  // Code colors
  umap<color, font> cmtext;
  for(const color& ct : TEXT_COLORS){
    font f;
    for(const char c : _SYMBOLS)
      f[c] = color_char(font_base[c], ct, BKGD_COLOR).scale(scale);
    cmtext[ct] = f; }
  cmbkgd[BKGD_COLOR] = cmtext;

  // File and command bar colors
  vec<color> bkgds =
      {CMD_BAR_COLOR, FOCUS_FILE_BAR_COLOR, UNFOCUS_FILE_BAR_COLOR};
  for(const color& cb : bkgds){
    umap<color, font> cmtext;
    font f;
    for(const char c : _SYMBOLS)
      f[c] = color_char(font_base[c], BAR_TEXT_COLOR, cb).scale(scale);
    cmtext[BAR_TEXT_COLOR] = f;
    cmbkgd[cb] = cmtext; }
  Panel::fonts[scale] = cmbkgd; }

image Editor::color_char(const image& img, const color& ctext,
    const color& cbkgd){
  image r = img;
  for(int y = 0; y < img.height; ++y)
    for(int x = 0; x < img.width; ++x){
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

void Editor::clip(){
  Panel& p = *focus;
  Cursor& c = p.cursor;
  if(p.ymark == -1) return;
  int y0, yf, x0, xf;
  if(c.y < p.ymark || (c.y == p.ymark && c.x < p.xmark))
    y0 = c.y, yf = p.ymark, x0 = c.x, xf = p.xmark - 1;
  else if(c.y > p.ymark || (c.y == p.ymark && c.x > p.xmark))
    y0 = p.ymark, yf = c.y, x0 = p.xmark, xf = c.x - 1;
  else return;
  clipboard.clear();
  clipboard.pb("");
  while(y0 < yf || (y0 == yf && x0 <= xf)){
    if(x0 < p.text[y0].size()){
      clipboard.back() += str(1, p.text[y0][x0]);
      ++x0; }
    if(x0 == p.text[y0].size()){
      clipboard.pb("");
      ++y0;
      x0 = 0; } } }

void Editor::split_horizontal(){
  Panel& p = *focus;
  p.height = (p.height + default_line_height) / 2 - default_line_height;
  p.refresh_file_bar = true;
  p.refresh_divider = true;
  int i;
  for(i = 0; i < panels.size(); ++i)
    if(focus == &panels[i]) break;
  if(i + 1 == panels.size())
    panels.pb(p);
  else
    panels.insert(panels.begin() + i + 1, p);
  Panel& p2 = panels[i];
  focus = &panels[i + 1];
  focus->pos = point(p2.pos.x, p2.pos.y + p2.height + default_line_height);
  refresh_panel(); }

void Editor::split_vertical(){
  Panel& p = *focus;
  p.width = (p.width + VERTICAL_DIVIDE) / 2 - VERTICAL_DIVIDE;
  p.refresh_file_bar = true;
  p.refresh_divider = true;
  int i;
  for(i = 0; i < panels.size(); ++i)
    if(focus == &panels[i]) break;
  if(i + 1 == panels.size())
    panels.pb(p);
  else
    panels.insert(panels.begin() + i + 1, p);
  Panel& p2 = panels[i];
  focus = &panels[i + 1];
  focus->pos = point(p2.pos.x + p2.width + VERTICAL_DIVIDE, p2.pos.y);
  refresh_panel(); }

void Editor::close_panel(){
  Panel& p = *focus;
  //!

}

#endif

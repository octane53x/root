// EDITOR

// Depends on libpng/zlib - loaded in Visual Studio

//! Test inputs when command bar in focus

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

  void init_members(const HINSTANCE wp1, const int wp2);

  virtual void init();
  virtual void run();
  virtual void update(const double ms);
  // Defined in input.hh
  virtual void process_key(const str& key, const bool down,
      const ipoint& mouse);

  // Defined in draw.hh
  void draw();
  // Defined in cmd.hh
  void process_cmd(const str& cmd);

  void resize(const ipoint& npos, const ipoint& nsize);
  void load_font();
  void color_font(const double scale);
  image color_char(const image& img, const color& ctext, const color& cbkgd);
  void scale_font(const double factor);
  void clip();
  void switch_panel(Panel* p);

  // Defined in input.hh
  bool parse_char(const str& key);
  void proc_indent();
  void proc_backspace();
  void proc_enter();
  void proc_escape();
  void proc_ctrl_move(const Dir d);
  void proc_ctrl_backspace();
  void proc_delete();
  void proc_del_space();
  void proc_open_file();
  void proc_set_mark();
  void proc_select_all();
  void proc_indent_selection();
  void proc_unindent_selection();
  void proc_cut();
  void proc_copy();
  void proc_paste();
  void proc_move_max(const Dir d);
  void proc_left_panel();
  void proc_right_panel();
  void proc_split_horizontal();
  void proc_split_vertical();
  void proc_close_panel(); };

void Editor::init_members(const HINSTANCE wp1, const int wp2){
  win_param_1 = wp1, win_param_2 = wp2; }

void Editor::init(){
  system::init();
  _win = this;
  shift = ctrl = alt = false;
  Panel::frame = Cursor::frame = &frame;

  // Window init
  RECT rect;
  SystemParametersInfo(SPI_GETWORKAREA, 0, &rect, 0);
  size = ipoint(2 * (PANEL_CHARS * CHAR_WIDTH_SCALE_1)
      + VERTICAL_DIVIDE + PANEL_OFFSET, rect.bottom - rect.top);
  win_pos = ipoint(rect.right - size.x, 0);
  size.x += WINDOW_OFFSET;
  size.y += WINDOW_OFFSET;

  // Initial panel
  panels.pb(Panel());
  Panel& p = panels.back();
  p.size = ipoint(size.x - VERTICAL_DIVIDE + WIDTH_OFFSET,
      size.y - LINE_HEIGHT_SCALE_1 * 2 + HEIGHT_OFFSET);
  p.init();
  focus = &p;

  // Command bar
  cmd.size = ipoint(size.x, LINE_HEIGHT_SCALE_1);
  cmd.pos = ipoint(0, p.size.y + p.line_height);
  cmd.init();
  cmd.cmd = true;
  cmd.bkgd = cmd.cursor.bkgd = CMD_BAR_COLOR;
  cmd.focus = cmd.cursor.blink = false;

  // Font
  load_font();
  color_font(1.0);

  // Display frame
  frame.set_size(size.x, size.y);
  draw(); }

void Editor::run(){
  system::run();
  for(Panel& p : panels){
    p.run();
    p.cursor.run(); }
  cmd.run();
  cmd.cursor.run();
  _win_init();
  _win_run(); }

void Editor::update(const double ms){
  system::update(ms);
  for(Panel& p : panels){
    p.update(ms);
    if(p.updated)
      updated = true; }
  cmd.update(ms);
  if(cmd.updated)
    updated = true;
  if(updated)
    last_update = clock(); }

void Editor::resize(const ipoint& npos, const ipoint& nsize){
  if(nsize.x == size.x && nsize.y == size.y){
    win_pos = npos;
    return; }
  frame.set_size(npos.x, npos.y);
  for(int y = 0; y < npos.y; ++y)
    for(int x = 0; x < npos.x; ++x)
      frame.data[y][x] = BKGD_COLOR;
  double xr = (double)nsize.x / size.x;
  double yr = (double)nsize.y / size.y;
  for(Panel& p : panels){
    p.pos.x = (int)round(p.pos.x * xr);
    p.pos.y = (int)round(p.pos.y * yr);
    p.size.x = (int)round(xr * p.size.x);
    p.size.y = (int)round(yr * p.size.y); }
  win_pos = npos, size = nsize; }

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
  //setjmp(png_jmpbuf(png_ptr));
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
  font_img.set_size(size.x, size.y);
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

void Editor::scale_font(const double factor){
  Panel& p = *focus;
  Cursor& c = p.cursor;
  p.text_scale *= factor;
  c.size.y = p.line_height = (int)ceil(p.text_scale * LINE_HEIGHT_SCALE_1);
  c.size.x = p.char_width = (int)ceil(p.text_scale * CHAR_WIDTH_SCALE_1);
  color_font(p.text_scale); }

void Editor::clip(){
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
      ++x0; }
    if(x0 == p.text[y0].size()){
      clipboard.pb("");
      ++y0;
      x0 = 0; } } }

void Editor::switch_panel(Panel* p){
  Panel& p0 = *focus;
  Cursor& c0 = p0.cursor;
  c0.blink = false;
  c0.fill = p0.bkgd;
  char ch = (c0.pos.x == p0.text[c0.pos.y].size())
      ? ' ' : p0.text[c0.pos.y][c0.pos.x];
  color tc = (ch == ' ') ? COLOR_CODE : p0.text_color[c0.pos.y][c0.pos.x];
  p0.draw_char(Panel::fonts[p0.text_scale][c0.fill][tc][ch],
      p0.text_to_frame(c0.pos));
  c0.draw(p0.text_to_frame(c0.pos));
  p0.focus = false;
  if(&p0 != &cmd && p != &cmd)
    p0.draw_file_bar();
  focus = p;
  p->focus = true;
  if(&p0 != &cmd && p != &cmd)
    p->draw_file_bar(); }

#endif

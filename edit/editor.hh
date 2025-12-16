// EDITOR

#ifndef EDITOR_HH
#define EDITOR_HH

#include "../os/win/util.hh"
#include "../os/win/window.hh"
#include "text_view.hh"

const str
    _FONT_LOC = "../symbols.bmp",
    _SYMBOLS = "0123456789ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz"
        ".,/?:;'\"+=-_\\|~!@#$%^&*()[]{}<>";

struct Editor : virtual window {

  bool shift, ctrl, alt;
  umap<char, image> font;
  TextView scene_text;

  Editor();

  virtual void init();
  virtual void run();
  virtual void update(const double ms);

  void load_font();
  void process_key(env::key_event ke); };

Editor::Editor(): shift(false), ctrl(false), alt(false) {
  type = "Editor"; }

void Editor::init(){
  load_font();
  scene_text.font = font;
  scene_text.init();
  window::init(); }

void Editor::run(){
  scene_text.run();
  window::run(); }

void Editor::update(const double ms){
  while(!keys.empty()){
    env::key_event ke = keys.front();
    keys.pop();
    process_key(ke); }
  window::update(ms);
  last_update = clock(); }

void Editor::load_font(){
  image font_img = load_bmp(_FONT_LOC);
  assert(font_img.width == _SYMBOLS.size() * CHAR_WIDTH, "Editor.load_font",
      "font image not aligned with symbol list");
  for(int i = 0; i < _SYMBOLS.size(); ++i){
    image new_char(CHAR_WIDTH, LINE_HEIGHT);
    for(int xi = i * CHAR_WIDTH, xo = 0; xo < CHAR_WIDTH; ++xi, ++xo)
      for(int y = 0; y < LINE_HEIGHT; ++y)
        new_char.set_pixel(xo, y, font_img.data[y][xi]);
    font[_SYMBOLS[i]] = new_char; } }

void Editor::process_key(env::key_event ke){
  // 0-9, A-Z, a-z
  if(ke.key.size() == 1){
    char ci = ke.key[0], co;
    if(ci >= '0' && ci <= '9')
      co = ci;
    else{
      assert(ci >= 'A' && ci <= 'Z', "Editor.process_key", "bad character");
      if(shift)
        co = ci;
      else
        co = ci - 'A' + 'a'; }
    str line = scene_text.text[scene_text.line_pos];
    scene_text.text[scene_text.line_pos] = line.substr(0, scene_text.char_pos)
        + str("" + co) + line.substr(scene_text.char_pos);
    ++scene_text.char_pos;

  // Modifiers
  }else if(ke.key == "SHIFT")
    shift = ke.down;
  else if(ke.key == "CONTROL")
    ctrl = ke.down;
  else if(ke.key == "ALT")
    alt = ke.down;

  //! more
}

int scene::win_w, scene::win_h;
umap<str, font> scene::fonts;

#endif

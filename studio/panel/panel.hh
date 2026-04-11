// STUDIO PANEL

#ifndef PANEL_HH
#define PANEL_HH

#include "cursor.hh"

// Abstract panel
struct Panel {

  // Whether the panel is in focus
  bool focus;
  // Frame position and pixel size
  ipoint pos, size;
  // Background color
  color bkgd;

  virtual bool init() = 0;
  virtual bool update() = 0;
  virtual bool draw() = 0;
  virtual bool input(const KeyEvent& ke) = 0; };

// Panel that displays text in lines
struct TextPanel : Panel {

  // Line index in text at top of panel
  int top_line;
  // Font size value supplied to TextOut
  int font_size;
  // Pixel size of characters
  ipoint char_size;
  // Text displayed in the panel, by line
  vec<str> text;
  // Color of each character
  vec<vec<color> > text_color;

  // Defined in this file
  ipoint text_to_pixel(const ipoint& p) const;

  // Defined in ../draw.hh
  virtual void draw() = 0;
  void draw_char(const char c, const ipoint& p);

  // Defined in text.hh
  void insert_text(const vec<str>& ins, const ipoint& p);
  void remove_text(const ipoint& p0, const ipoint& pf);
  void clean(); };

// Panel that allows text input
struct InputPanel : TextPanel {

  // Text operation stored for undo/redo
  struct TextOp {
    bool ins; // False if remove
    ipoint pos;
    vec<str> text; };

  // Text position of selection mark
  ipoint mark;
  // Text operation stacks
  vec<TextOp> undo_stack, redo_stack;
  // Cursor where text is modified
  Cursor cursor;

  // Defined in this file
  virtual void init();
  virtual void update();
  // Defined in ../draw.hh
  virtual void draw();
  // Defined in ../input.hh
  virtual void input(const KeyEvent& ke);

  // Defined in text.hh
  void newline();
  void del();
  void del_space();
  void del_back();
  void del_back_word();
  void del_selection();
  void indent();
  void indent_selection();
  void unindent_selection();
  void undo();
  void redo();

  // Defined in move.hh
  void move_up();
  void move_down();
  void move_left();
  void move_right();
  void move_up_block();
  void move_down_block();
  void move_left_word();
  void move_right_word();
  void move_up_max();
  void move_down_max();
  void move_left_max();
  void move_right_max();
  void goto_line(const int n);

  // Defined in select.hh
  void set_mark();
  void select_line();
  void select_block();
  void select_all(); };

#endif

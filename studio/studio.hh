// DEVELOPMENT STUDIO

#ifndef STUDIO_HH
#define STUDIO_HH

#include "../os/win/application.hh"
#include "../edit/editor.hh"
#include "../term/terminal.hh"

// Windowed application with code editor and terminal panels
struct Studio : Application {

  // Text clipboard for cut/copy/paste
  vec<str> clipboard;
  // Panels
  Panel *focus, *prev;
  vec<Panel*> panels;
  CmdBar cmd;
  InfoPanel info;

  // Defined in this file
  virtual void init();
  virtual void update();
  // Defined in draw.hh
  virtual void draw();
  // Defined in input.hh
  virtual void input(const KeyEvent& ke);

  // Defined in ops/layout.hh
  virtual void resize();
  void switch_panel(Panel* p);
  void left_panel();
  void right_panel();
  void split_horizontal();
  void split_vertical();
  void close_panel();

  // Defined in ops/clip.hh
  void clip();
  void cut();
  void copy();
  void paste(); };

// Initialize Studio
// Called by: wWinMain()
void Studio::init(){
  // Set default member state
  Application::init();
  start_maximized = false;

  // Find biggest screen
  GetScreens screens;
  assert(screens.rects.size() < 1, "Studio.init", "Zero screens detected");
  int mi = -1, mx = -1;
  for(int i = 0; i < screens.rects.size(); ++i){
    const RECT& r = screens.rects[i];
    int area = (r.right - r.left) * (r.bottom - r.top);
    if(area > mx){
      mx = area;
      mi = i; } }
  RECT rect = screens.rects[mi];

  // Find taskbar height
  RECT work_area;
  SystemParametersInfo(SPI_GETWORKAREA, 0, &work_area, 0);
  int taskbar_height = GetSystemMetrics(SM_CYSCREEN)
      - (work_area.bottom - work_area.top);

  // Set window size and position
  frame_size = ipoint(2 * ((PANEL_CHARS * CHAR_WIDTH_SCALE_1)
      + VERTICAL_DIVIDE),
      rect.bottom - rect.top - taskbar_height + FRAME_HEIGHT_OFFSET);
  size = ipoint(frame_size.x + WIN_WIDTH_OFFSET,
      frame_size.y + WIN_HEIGHT_OFFSET);
  pos = ipoint(rect.right - size.x + WIN_XPOS_OFFSET, rect.top);
  frame.set_size(frame_size);

  // Initial panel
  panels.pb(Panel());
  Panel& p = panels.back();
  p.size = ipoint(frame_size.x - VERTICAL_DIVIDE,
      frame_size.y - 2 * LINE_HEIGHT_SCALE_1);
  p.init();
  focus = &p;

  // Command bar
  cmd_panel.size = ipoint(frame_size.x, LINE_HEIGHT_SCALE_1);
  cmd_panel.pos = ipoint(0, p.size.y + p.line_height);
  cmd_panel.init();
  cmd_panel.cmd = true;
  cmd_panel.bkgd = cmd_panel.cursor.bkgd = CMD_BAR_COLOR;
  cmd_panel.focus = cmd_panel.cursor.blink = false;

  // Font
  load_font();
  color_font(1.0);

  // Preferences
  process_cmd("Open: C:/home/root/edit/editor.hh");
  split_vertical();
  process_cmd("Open: C:/home/root/edit/input.hh");
  left_panel(); }

// Update the Studio
// Called by: msg_proc()
void Studio::update(){
  Application::update();
  for(Panel* p : panels)
    p->update();
  cmd.update(); }

#endif

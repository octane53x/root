// ABSTRACT Windows Application

//! TODO
// 2 Threads:
// 1 repeatedly calls update()
// 1 handles Windows msg loop and calls BitBlt

#ifndef WIN_APP_HH
#define WIN_APP_HH

#include "window.hh"

// Window I/O handler
struct Application : virtual Window {

  // Whether key modifiers are held down
  bool shift, ctrl, alt;

  virtual void init();
  virtual void update();
  virtual void run();

  void parse_key(KeyEvent& ke, bool _shift);
  void display_text(
      const str& text, const int size, const str& font, const point pos,
      const color c); };

// Set default member state
// Called by: PROJECT init()
void Application::init(){
  Window::init();
  shift = ctrl = alt = false;
  last_update = last_draw = 0; }

// Send key events to derived application
// Returns true if key events were routed
// Called by: PROJECT update()
void Application::update(){
  Interface::update();
  for(KeyEvent& ke : keys){
    if(ke.key == "SHIFT")
      shift = ke.down;
    else if(ke.key == "CONTROL")
      ctrl = ke.down;
    else if(ke.key == "ALT")
      alt = ke.down;
    ke.ctrl = ctrl, ke.alt = alt;
    parse_key(ke, shift);
    if(input(ke))
      updated = true; }
  keys.clear(); }

// Display window and run main loop
// Called by: PROJECT
void Application::run(){
  win_size =
      point(frame.size.x - FRAME_X_OFFSET, frame.size.y - FRAME_Y_OFFSET);
  display();
  // Windows increases this timer resolution to its minimum (~16ms?)
  SetTimer(hWnd, IDT_TIMER1, 1 /*ms*/, NULL);
  MSG msg = {};
  while(GetMessage(&msg, NULL, 0, 0)){
    TranslateMessage(&msg);
    DispatchMessage(&msg); } }

// Considering _SHIFT, translate key sent by Window
// Called by: update()
void Application::parse_key(KeyEvent& ke, bool _shift){
  char co = 0;
  if(ke.key.size() == 1){
    char ci = ke.key[0];
    if(ci >= '0' && ci <= '9'){
      if(_shift){
        switch(ci){
        case '0': co = ')'; break;
        case '1': co = '!'; break;
        case '2': co = '@'; break;
        case '3': co = '#'; break;
        case '4': co = '$'; break;
        case '5': co = '%'; break;
        case '6': co = '^'; break;
        case '7': co = '&'; break;
        case '8': co = '*'; break;
        case '9': co = '('; break;
        default: break; }
      }else
        co = ci;
    }else{
      assert(ci >= 'A' && ci <= 'Z', "Application.parse_key", "bad character");
      co = _shift ? ci : ci - 'A' + 'a'; }

  }else if(ke.key == "COLON") co = _shift ? ':' : ';';
  else if(ke.key == "EQUALS") co = _shift ? '+' : '=';
  else if(ke.key == "COMMA") co = _shift ? '<' : ',';
  else if(ke.key == "MINUS") co = _shift ? '_' : '-';
  else if(ke.key == "PERIOD") co = _shift ? '>' : '.';
  else if(ke.key == "SLASH") co = _shift ? '?' : '/';
  else if(ke.key == "TILDE") co = _shift ? '~' : '`';
  else if(ke.key == "LBRACKET") co = _shift ? '{' : '[';
  else if(ke.key == "BACKSLASH") co = _shift ? '|' : '\\';
  else if(ke.key == "RBRACKET") co = _shift ? '}' : ']';
  else if(ke.key == "QUOTE") co = _shift ? '"' : '\'';
  else if(ke.key == "SPACE") co = ' ';
  if(co != 0)
    ke.key = str(1, co); }

// Display text on screen
// Called by: PROJECT
void Application::display_text(
    const str& text, const int size, const str& font, const point pos,
    const color c){
  HFONT f = CreateFont(
      size, 0, 0, 0, FW_BOLD, FALSE, FALSE, FALSE,
      DEFAULT_CHARSET, OUT_TT_ONLY_PRECIS, CLIP_DEFAULT_PRECIS,
      PROOF_QUALITY, FIXED_PITCH | FF_MODERN, font.c_str());
  HFONT fOld = (HFONT)SelectObject(hdcMem, f);
  SetTextColor(hdcMem, RGB(c.r, c.g, c.b));
  SetBkMode(hdcMem, TRANSPARENT);
  TextOut(hdcMem, pos.x, pos.y, text.c_str(), text.size());
  SelectObject(hdcMem, fOld); }

#endif

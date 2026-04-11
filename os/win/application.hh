// ABSTRACT Windows Application

//! TODO
// 2 Threads:
// 1 repeatedly calls update()
// 1 handles Windows msg loop and calls BitBlt

#ifndef WIN_APP_HH
#define WIN_APP_HH

#include "util.hh"
#include "window.hh"

// Window I/O handler
struct Application : virtual Window {

  // Whether key modifiers are held down
  bool shift, ctrl, alt;
  // Time of prior update/draw completion
  clock_t last_update, last_draw;

  virtual void init();
  virtual void update();
  virtual void draw() = 0;
  virtual void run();

  void parse_key(KeyEvent& ke); };

// Set default member state
// Called by: PROJECT init()
void Application::init(){
  Window::init();
  shift = ctrl = alt = false;
  max_fps = MAX_FPS;
  last_update = last_draw = 0; }

// Send key events to derived application
// Returns true if key events were routed
// Called by: PROJECT update()
void Application::update(){
  key_router.update();
  for(KeyEvent ke : key_router.keys){
    if(ke.key == "SHIFT")
      shift = ke.down;
    else if(ke.key == "CONTROL")
      ctrl = ke.down;
    else if(ke.key == "ALT")
      alt = ke.down;
    ke.shift = shift, ke.ctrl = ctrl, ke.alt = alt;
    parse_key(ke);
    input(ke); }
  key_router.keys.clear();
  last_update = clock(); }

// Display window and run main loop
// Called by: PROJECT
void Application::run(){
  display();
  // Windows increases this timer resolution to its minimum (~16ms?)
  SetTimer(hWnd, IDT_TIMER1, 1 /*ms*/, NULL);
  MSG msg = {};
  while(GetMessage(&msg, NULL, 0, 0)){
    TranslateMessage(&msg);
    DispatchMessage(&msg); } }

// Considering SHIFT, translate key sent by Window
// Called by: update()
void Application::parse_key(KeyEvent& ke){
  char co = 0;
  if(ke.key.size() == 1){
    char ci = ke.key[0];
    if(ci >= '0' && ci <= '9'){
      if(ke.shift){
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
      co = ke.shift ? ci : ci - 'A' + 'a'; }

  }else if(ke.key == "COLON") co = ke.shift ? ':' : ';';
  else if(ke.key == "EQUALS") co = ke.shift ? '+' : '=';
  else if(ke.key == "COMMA") co = ke.shift ? '<' : ',';
  else if(ke.key == "MINUS") co = ke.shift ? '_' : '-';
  else if(ke.key == "PERIOD") co = ke.shift ? '>' : '.';
  else if(ke.key == "SLASH") co = ke.shift ? '?' : '/';
  else if(ke.key == "TILDE") co = ke.shift ? '~' : '`';
  else if(ke.key == "LBRACKET") co = ke.shift ? '{' : '[';
  else if(ke.key == "BACKSLASH") co = ke.shift ? '|' : '\\';
  else if(ke.key == "RBRACKET") co = ke.shift ? '}' : ']';
  else if(ke.key == "QUOTE") co = ke.shift ? '"' : '\'';
  else if(ke.key == "SPACE") co = ' ';
  if(co != 0)
    ke.key = str(1, co); }

#endif

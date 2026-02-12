// KEY ROUTER
// Owned by Window

#ifndef KEY_ROUTER_HH
#define KEY_ROUTER_HH

#include "../../core/util.hh"

// Key hold constants
const double
    KEY_INITIAL_DELAY = 0.3,
    KEY_REPEAT_DELAY = 0.02;

// Pushed to a queue when any key is pressed or released
struct KeyEvent {
  // Key identifier
  str key;
  // Key down if true, up if false
  bool down;
  // Cursor pos at event time
  ipoint cursor;
  // Time of event
  clock_t time;

  KeyEvent(const str& k, const bool d, const ipoint& c, const clock_t t); };

struct Application;

// Stores key events and creates held key repeat events
struct KeyRouter {

  // Repeat held key
  bool key_repeat;
  // Time of last input sent to derived application
  clock_t last_send;
  // Pixel position of cursor
  ipoint cursor;
  // Key held down
  str key_down;
  // Key event queue, processed on application update
  vec<KeyEvent> keys;

  void init();
  void update();
  void send(const KeyEvent& ke); };

// Set default member state
// Called by: Window.msg_proc(), route()
KeyEvent::KeyEvent(
    const str& k, const bool d, const ipoint& c, const clock_t t):
    key(k), down(d), cursor(c), time(t) {}

// Set default member state
// Called by: Window.init()
void KeyRouter::init(){
  key_repeat = false;
  last_send = 0; }

// Add the held key to keys queue if time has elapsed
// Called by: Application.update()
void KeyRouter::update(){
  // Set held key
  for(const KeyEvent& ke : keys){
    if(ke.down)
      key_down = ke.key;
    else if(ke.key == key_down){
      key_down = "";
      key_repeat = false; } }
  if(key_down != ""){
    clock_t now = clock();
    if((double)(now - last_send) / CPS > KEY_INITIAL_DELAY)
      key_repeat = true;
    if(key_repeat && (double)(now - last_send) / CPS > KEY_REPEAT_DELAY){
      keys.pb(KeyEvent(key_down, true, cursor, now));
      last_send = now; } } }

// Send a new key event
// Called by: Window.msg_proc()
void KeyRouter::send(const KeyEvent& ke){
  if(ke.down && ke.key == key_down)
    return;
  keys.pb(ke);
  last_send = ke.time; }

#endif

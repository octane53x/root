// ENVIRONMENT

// gl/button and gl/scene constructors defined in this file so they can
// automatically add their pointers to the env

#ifndef ENVIRONMENT_HH
#define ENVIRONMENT_HH

#include "button.hh"
#include "scene.hh"

// Window position is slightly different than frame position
const int
    WIN_OFFSET_X = 8,
    WIN_OFFSET_Y = 32;

struct env;

// Set in constructor. Enables buttons and scenes to add themselves.
env* _env;

// The I/O handler to a window, which inherits this. I/O includes:
// Mouse movement, button hover and click, pressing keys, and
// drawing scenes into a frame of pixels to send the window
struct env : virtual object {

  // Key press or release event
  struct key_event {
    // True if key press, false if key release
    bool down;
    // Key name
    str key;
    // Where the mouse was at the time of the event
    point cursor; };

  // Current frames per second
  int fps;
  // Last time the frame was drawn
  clock_t last_frame;
  // Frame reset to this color at the beginning of each draw
  color bkgd_color;
  // Mouse cursor position
  point cursor;
  // All the key events received since last update
  queue<key_event> keys;
  // The frame image drawn to, and its background it resets to first
  image frame, bkgd;
  // All buttons in all scenes, adding here upon construction
  umap<llu, button*> buttons;
  // All scenes, adding here upon construction
  umap<llu, scene*> scenes;

  env();

  virtual void validate(const str& func);
  virtual void init();
  virtual void update(const double ms);
  virtual void draw(image* canvas, const viewport& view);

  void draw_bkgd();
  void hover();
  void click(const point& p); };

// Set default member state and global env pointer
env::env(): bkgd_color(DEFAULT_COLOR) {
  type = "env";
  last_frame = clock();
  _env = this; }

// FOREIGN CONSTRUCTOR
// Add itself to env automatically upon creation
button::button(){
  type = "button";
  _env->buttons[id] = this; }

// FOREIGN CONSTRUCTOR
// Add itself to env automatically upon creation
scene::scene(): z(0.0), bkgd_color(DEFAULT_COLOR) {
  type = "scene";
  _env->scenes[id] = this; }

// Ensure valid state
void env::validate(const str& func){
  system::validate(func);
  bkgd.validate(func);
  frame.validate(func);
  assert(bkgd.width == scene::win_w && bkgd.height == scene::win_h, func,
      "env.bkgd size not equal to window");
  assert(frame.width == scene::win_w && frame.height == scene::win_h, func,
      "env.frame size not equal to window"); }

// Fill the background with debug color, seen if scenes are positioned wrong
// Called by: window.init
void env::init(){
  system::init();
  draw_bkgd();
  frame = bkgd; }

// Update active scenes and hover a button if eligible
// Called by: PROJECT
void env::update(double ms){
  system::update(ms);
  for(pair<llu, scene*> s : scenes)
    if(s.second->active)
      s.second->update(ms);
  hover();
  last_update = clock();
  validate("env.update"); }

// Draw active scenes to frame
// Parameters unused, required from inheritance
// Called by: window.main_loop
void env::draw(image* canvas, const viewport& view){
  vec<scene*> active_scenes;
  for(pair<llu, scene*> p : scenes)
    if(p.second->active) active_scenes.pb(p.second);
  sort(active_scenes.begin(), active_scenes.end(),
      [](const scene* a, const scene* b){ return a->z > b->z; });
  frame = bkgd;
  viewport default_view;
  default_view.size = min(frame.width, frame.height);
  //! Adjust view by pos
  for(int i = 0; i < active_scenes.size(); ++i)
    active_scenes[i]->draw(&frame, default_view);
  clock_t now = clock();
  fps = (int)floor(1.0 / ((double)(now - last_frame) / CLOCKS_PER_SEC));
  last_frame = now;
  validate("env.draw"); }

// Draw the background image
// Called by: env.init, window.update_pos
void env::draw_bkgd(){
  int w = scene::win_w, h = scene::win_h;
  bkgd.set_size(w, h);
  for(int i = 0; i < h; ++i)
    for(int j = 0; j < w; ++j)
      bkgd.set_pixel(j, i, bkgd_color); }

// Hover a button, attempted in order of z value
// Called by: update
void env::hover(){
  vec<button*> v;
  for(pair<llu, button*> p : buttons)
    v.pb(p.second);
  sort(v.begin(), v.end(),
      [](const button* a, const button* b){ return a->pos.z < b->pos.z; });
  for(int i = 0; i < v.size(); ++i){
    bool hovered = v[i]->hover(cursor);
    if(hovered) break; }
  validate("env.hover"); }

// Click a button, attempted in order of z value
// Called by: PROJECT
void env::click(const point& p){
  vec<button*> v;
  for(pair<llu, button*> p : buttons)
    v.pb(p.second);
  sort(v.begin(), v.end(),
      [](const button* a, const button* b){ return a->pos.z < b->pos.z; });
  for(int i = 0; i < v.size(); ++i){
    bool clicked = v[i]->click(p);
    if(clicked) break; }
  validate("env.click"); }

#endif

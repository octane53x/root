// TITLE SCENE

#ifndef TITLE_HH
#define TITLE_HH

#include "../../gl/polygon.hh"
#include "../../gl/label.hh"
#include "../../gl/button.hh"
#include "../../gl/scene.hh"

// Title scene displayed upon window execution
struct Title : virtual scene {

  // Play button
  struct PlayBtn : virtual button {
    virtual void hover_fn();
    virtual void click_fn();
  } play_btn;

  // Whether the player is logged in
  bool logged_in;
  //! Temporary
  clock_t last_update_1;
  // "IMPACT" text
  label title_lbl;
  //! Temporary
  polygon* rand_color_box;

  Title();

  virtual void validate(const str& func);
  virtual void init();
  virtual void update(const double ms);

  void draw_bkgd();
  void load(); };

// Set default member state
Title::Title(): logged_in(false), last_update_1(0) {
  type = "Title"; }

// Ensure valid state
void Title::validate(const str& func){
  scene::validate(func);
  play_btn.validate(func); }

// Initialize objects in scene
void Title::init(){
  width = win_w, height = win_h;
  bkgd_color = RED;

  title_lbl.text = "IMPACT";
  title_lbl.fill = BLACK;
  title_lbl.size = 40;
  title_lbl.pos = point(100, 100);
  title_lbl.font = &fonts["aldo"];
  // Static, part of bkgd, does not need to be added to objs

  play_btn.fill = BLACK;
  play_btn.add(point(400, 300));
  play_btn.add(point(400, 400));
  play_btn.add(point(350, 350));
  objs[play_btn.id] = &play_btn;

  polygon* a = new polygon();
  a->points = {point(0,0), point(10,0), point(10,10), point(0,10)};
  a->fill = BLUE;
  a->pos = point(250,150);
  a->mov = new object::movement(object::movement::PATH);
  a->mov->path = {point(100,100), point(-100,100), point(-100,-100),
      point(100,-100)};
  a->mov->vel = 50.0;
  objs[a->id] = a;

  polygon* b = new polygon();
  b->points = a->points;
  b->fill = YELLOW;
  b->pos = point(250,100);
  b->mov = new object::movement(object::movement::ORBIT);
  b->mov->root = a;
  b->mov->vel = 100.0;
  objs[b->id] = b;

  polygon* c = new polygon();
  c->points = a->points;
  c->fill = GREEN;
  c->pos = point(250,125);
  c->mov = new object::movement(object::movement::ORBIT);
  c->mov->root = b;
  c->mov->vel = 200.0;
  rand_color_box = c;
  objs[c->id] = c;

  scene::init();
  draw_bkgd(); }

// Move the boxes
void Title::update(const double ms){
  scene::update(ms);
  clock_t now = clock();
  if((double)(now - last_update_1) / CLOCKS_PER_SEC >= 0.1){
    rand_color_box->fill = color().random();
    last_update_1 = now; }
  validate("Title.update"); }

// Draw background
void Title::draw_bkgd(){
  title_lbl.draw(&bkgd, view); }

// Enter loading state
void Title::load(){
  play_btn.stop(); }

#endif

// SCENE

#ifndef SCENE_HH
#define SCENE_HH

#include "object.hh"
#include "font.hh"
#include "camera.hh"

// Holds graphical elements, contained by env
struct scene : virtual object {

  // Tree node used to move rooted objects
  struct move_node {
    object* obj;
    move_node* parent;
    vec<move_node*> children; };

  // Scene size
  int width, height;
  // Window size available here so scenes can resize themselves properly
  static int win_w, win_h;
  // Z position of scene to decide which scenes are in front of the others
  double z;
  // Background color of scene, reset to before drawing
  color bkgd_color;
  // Available fonts for label text
  static umap<str, font> fonts;
  // ALL OBJECTS IN THE SCENE - TO MOVE, UPDATE, DRAW
  umap<llu, object*> objs;
  // Canvas to draw to, and its background it resets to first
  image bkgd, img;
  // Translation window that displays the desired part of the scene
  viewport vp;
  // Point and direction of view into the scene
  camera cam;

  scene();

  virtual void validate(const str& func);
  virtual void init();
  virtual void run();
  virtual void stop();
  virtual void update(const double ms);
  virtual void draw(image* canvas, const viewport& view);
  virtual void draw_bkgd();

  void move_rec(const move_node* node, const point& mov, const double ms);
  void move_objs(const double ms); };

// Ensure valid state
void scene::validate(const str& func){
  object::validate(func);
  bkgd.validate(func);
  img.validate(func);
  vp.validate(func);
  cam.validate(func);
  assert(width > 0 && height > 0, func, "scene size not positive"); }

// Prepare scene for first update and draw
// Called by: DERIVED CLASS
void scene::init(){
  object::init();
  vp.size_in = vp.size_out = min(width, height);
  draw_bkgd();
  img = bkgd;
  for(pair<llu, object*> p : objs)
    if(!p.second->initialized)
      p.second->init(); }

// Activate objects
void scene::run(){
  object::run();
  for(pair<llu, object*> p : objs)
    if(!p.second->active)
      p.second->run();
  validate("scene.run"); }

// Deactivate objects
void scene::stop(){
  object::stop();
  for(pair<llu, object*> p : objs)
    if(p.second->active)
      p.second->stop();
  validate("scene.stop"); }

// Update and move objects
// Called by: DERIVED CLASS
void scene::update(const double ms){
  object::update(ms);
  move_objs(ms);
  for(pair<llu, object*> p : objs)
    if(p.second->active)
      p.second->update(ms);
  last_update = clock();
  validate("scene.update"); }

// Draw objects onto background
// Called by: env.draw
void scene::draw(image* canvas, const viewport& view){
  img = bkgd;
  vec<object*> v;
  for(pair<llu, object*> p : objs)
    if(p.second->active)
      v.pb(p.second);
  sort(v.begin(), v.end(),
      [](const object* a, const object* b){ return a->pos.z > b->pos.z; });
  for(int i = 0; i < v.size(); ++i)
    v[i]->draw(&img, vp);
  img.draw(canvas, view);
  validate("scene.draw"); }

// Draw scene background
void scene::draw_bkgd(){
  bkgd.set_size(width, height);
  img.set_size(width, height);
  for(int i = 0; i < height; ++i)
    for(int j = 0; j < width; ++j)
      bkgd.set_pixel(j, i, bkgd_color); }

// Recursive helper function to move_objs
// For objects rooted to objects that are also rooted to other objects
// Called by: move_objs
void scene::move_rec(const move_node* node, const point& mov, const double ms){
  node->obj->pos += mov;
  point mov2 = node->obj->move(ms);
  for(int i = 0; i < node->children.size(); ++i)
    move_rec(node->children[i], mov + mov2, ms);
  validate("scene.move_rec"); }

// Move objects along movement patterns. Custom movement done in derived class
// Cannot be done in the object's own update because the movement depends on
// other objects held here
// Called by: update
void scene::move_objs(const double ms){
  vec<move_node> nodes;
  umap<llu, int> m;
  for(pair<llu, object*> p : objs){
    move_node node;
    node.obj = p.second;
    node.parent = NULL;
    nodes.pb(node);
    m[p.second->id] = (int)nodes.size() - 1; }
  for(pair<llu, object*> p : objs){
    if(p.second->mov == NULL || p.second->mov->root == NULL) continue;
    assert(m.find(p.second->mov->root->id) != m.end(), "scene.move_objs",
        "obj not found");
    assert(m.find(p.second->id) != m.end(), "scene.move_objs", "obj not found");
    int j = m[p.second->mov->root->id];
    int k = m[p.second->id];
    nodes[j].children.pb(&nodes[k]);
    nodes[k].parent = &nodes[j]; }
  for(int i = 0; i < nodes.size(); ++i)
    if(nodes[i].parent == NULL)
      move_rec(&nodes[i], point(0, 0, 0), ms);
  validate("scene.move_objs"); }

#endif

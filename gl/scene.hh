// SCENE

#ifndef SCENE_HH
#define SCENE_HH

#include "object.hh"
#include "font.hh"
#include "camera.hh"

// Holds graphical elements, contained by env
struct scene : object {

  // Tree node used to move rooted objects
  struct move_node {
    object* obj;
    move_node* parent;
    vec<move_node*> children; };

  bool active;
  int width, height;
  static int win_w, win_h;
  double z;
  clock_t last_frame;
  color bkgd_color;
  static umap<str, font> fonts;
  vec<object*> objects;
  image bkgd, img;
  viewport view;
  camera cam;

  scene();

  virtual void validate(str func);
  virtual void init();
  virtual void update(double ms);
  virtual void draw(image* frame, viewport view);

  void init_members();
  void resize_window();
  void move_rec(move_node* node, point mov, double ms);
  void move_objects(double ms);

// Set default member state
scene(): type("scene"), active(false), last_frame(0), z(0.0),
    bkgd_color(MAGENTA) {}

// Ensure valid state
void validate(str func){
  object::validate(func);
  assert(width > 0 && height > 0, "scene size not positive"); }

// Set the scene size
// Called by: DERIVED CLASS
void scene::init_members(int w, int h){
  width = w, height = h;
  validate("scene.init_members"); }

// Prepare scene for first update and draw
// Call init_members first
// Called by: DERIVED CLASS
void scene::init(){
  view.size = min(width, height);
  bkgd.set_size(width, height);
  for(int i = 0; i < height; ++i)
    for(int j = 0; j < width; ++j)
      bkgd.set_pixel(j, i, bkgd_color);
  frame = bkgd;
  validate("scene.init"); }

// Update and move objects
// Called by: DERIVED CLASS
void scene::update(double ms){
  move_objects(ms);
  for(object* o : objects)
    o->update(ms);
  validate("scene.update"); }

// Draw objects onto background
// Called by: env.draw
void scene::draw(image* frame, viewport view){
  img = bkgd;
  sort(objects.begin(), objects.end(),
      [](const object* a, const object* b){ return a->pos.z > b->pos.z; });
  for(int i = 0; i < objects.size(); ++i)
    objects[i]->draw(&img, view);
  for(int y = 0; y < height; ++y)
    for(int x = 0; x < width; ++x)
      frame->set_pixel(x + pos.x, y + pos.y, img.data[y][x]);
  validate("scene.draw"); }

// Signal a window resize so objects can adjust on next update
// Called by: PROJECT
void scene::resize_window(int w, int h){
  win_w = w, win_h = h;
  validate("scene.resize_window"); }

// Recursive helper function to move_objects
// For objects rooted to objects that are also rooted to other objects
// Called by: move_objects
void scene::move_rec(move_node* node, point mov, double ms){
  node->obj->pos += mov;
  point mov2 = node->obj->move(ms);
  for(int i = 0; i < node->children.size(); ++i)
    move_rec(node->children[i], mov + mov2, ms);
  validate("scene.move_rec"); }

// Move objects along movement patterns. Custom movement done in derived class
// Cannot be done in the object's own update because the movement depends on
// other objects held here
// Called by: update
void scene::move_objects(double ms){
  vec<move_node> nodes;
  umap<llu, int> m;
  for(int i = 0; i < objects.size(); ++i){
    move_node node;
    node.obj = objects[i];
    node.parent = NULL;
    nodes.pb(node);
    m[objects[i]->id] = (int)nodes.size() - 1; }
  for(int i = 0; i < objects.size(); ++i){
    if(objects[i]->mov == NULL || objects[i]->mov->root == NULL) continue;
    assert(m.find(objects[i]->mov->root->id) != m.end(), "obj not found");
    assert(m.find(objects[i]->id) != m.end(), "obj not found");
    int j = m[objects[i]->mov->root->id];
    int k = m[objects[i]->id];
    nodes[j].children.pb(&nodes[k]);
    nodes[k].parent = &nodes[j]; }
  for(int i = 0; i < nodes.size(); ++i)
    if(nodes[i].parent == NULL)
      move_rec(&nodes[i], point(0, 0, 0), ms);
  validate("scene.move_objects"); }

#endif

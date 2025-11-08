// SCENE

#ifndef SCENE_HH
#define SCENE_HH

#include "object.hh"
#include "font.hh"

bool zcompare(const object* a, const object* b){
  return a->pos.z > b->pos.z; }

struct scene : thing {

  struct move_node {

    object* obj;
    move_node* parent;
    vec<move_node*> children; };

  clock_t last_frame;
  int width, height;
  color bkgd_color;
  point pos;
  image bkgd, frame;
  umap<str, font*> fonts;
  vec<object*> objs;

  scene(int w, int h):
      last_frame(0), width(w), height(h), bkgd(w, h), frame(w, h) {}

  virtual void validate(){
    assert(width > 0 && height > 0, "scene size not positive"); }

  virtual void update(double ms) = 0;

  image* next_frame(){
    frame = bkgd;
    clock_t now = clock();
    double ms = (double)(now - last_frame) * 1000.0 / CLOCKS_PER_SEC;
    last_frame = now;
    update(ms);
    move_objs(ms);
    draw_objs();
    return &frame; }

  virtual void draw_bkgd(){
    for(int i = 0; i < height; ++i)
      for(int j = 0; j < width; ++j)
        bkgd.data[i][j] = bkgd_color; }

  void move_rec(move_node* node, point mov, double ms){
    node->obj->pos += mov;
    point mov2 = node->obj->move(ms);
    for(int i = 0; i < node->children.size(); ++i)
      move_rec(node->children[i], mov + mov2, ms); }

  void move_objs(double ms){
    vec<move_node> nodes;
    umap<llu, int> m;
    for(int i = 0; i < objs.size(); ++i){
      move_node node;
      node.obj = objs[i];
      node.parent = NULL;
      nodes.pb(node);
      m[objs[i]->id] = (int)nodes.size() - 1; }
    for(int i = 0; i < objs.size(); ++i){
      if(objs[i]->mov->root == NULL) continue;
      assert(m.find(objs[i]->mov->root->id) != m.end(), "obj not found");
      assert(m.find(objs[i]->id) != m.end(), "obj not found");
      int j = m[objs[i]->mov->root->id];
      int k = m[objs[i]->id];
      nodes[j].children.pb(&nodes[k]);
      nodes[k].parent = &nodes[j]; }
    for(int i = 0; i < nodes.size(); ++i)
      if(nodes[i].parent == NULL)
        move_rec(&nodes[i], point(0, 0, 0), ms); }

  void draw_objs(){
    sort(objs.begin(), objs.end(), zcompare);
    for(int i = 0; i < objs.size(); ++i)
      objs[i]->draw(&frame);
  } };

#endif

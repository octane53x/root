// MOVE

#ifndef MOVE_HH
#define MOVE_HH

#include "scene.hh"

point object::move(double ms){
  point move = point(0, 0);
  double dist = mov.vel * (ms / 1000.0);
  if(mov.type == mov_type::NONE || mov.type == mov_type::ROOT) return move;
  if(mov.type == mov_type::PATH){
    while(dist > 0.0){
      point p = mov.path[mov.path_pos];
      double step = p.dist(point(0, 0));
      double next = step - mov.path_prog;
      if(dist < next){
        move += p * (dist / step);
        mov.path_prog += dist;
        dist = 0.0;
      }else{
        move += p * (next / step);
        dist -= next;
        mov.path_prog = 0.0;
        ++mov.path_pos;
        if(mov.path_pos == mov.path.size()) mov.path_pos = 0; } }
  }else if(mov.type == mov_type::ORBIT){
    double r = pos.dist(mov.root->pos);
    double deg = dist / r;
    point p = pos;
    p.rotate(mov.root->pos, deg);
    move = p - pos; }
  pos += move;
  return move; }

void scene::move_rec(move_node* node, point mov, double ms){
  node->obj->pos += mov;
  point mov2 = node->obj->move(ms);
  for(int i = 0; i < node->children.size(); ++i)
    move_rec(node->children[i], mov + mov2, ms); }

void scene::move_objs(double ms){
  vec<move_node> nodes;
  umap<llu, int> m;
  for(int i = 0; i < objs.size(); ++i){
    move_node node;
    node.obj = objs[i];
    node.parent = NULL;
    nodes.pb(node);
    m[objs[i]->id] = (int)nodes.size() - 1; }
  for(int i = 0; i < objs.size(); ++i){
    if(objs[i]->mov.root == NULL) continue;
    assert(m.find(objs[i]->mov.root->id) != m.end(), "obj not found");
    assert(m.find(objs[i]->id) != m.end(), "obj not found");
    int j = m[objs[i]->mov.root->id];
    int k = m[objs[i]->id];
    nodes[j].children.pb(&nodes[k]);
    nodes[k].parent = &nodes[j]; }
  for(int i = 0; i < nodes.size(); ++i)
    if(nodes[i].parent == NULL)
      move_rec(&nodes[i], point(0, 0), ms); }

#endif

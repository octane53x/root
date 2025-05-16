// OBJ

#ifndef OBJ_HH
#define OBJ_HH

#include "thing.hh"

template <typename T>
struct graph : thing {
  vec<T> nodes;
  umap<int, int> edges;
  graph(){ type = "graph"; } };

template <typename T>
struct tree : graph<T> {
  T root;
  tree(){ this->type = "tree"; } };

#endif

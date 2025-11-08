// GRAPH

#ifndef GRAPH_HH
#define GRAPH_HH

#include "util.hh"

template <typename T>
struct graph : thing {

  struct node {

    bool vis;
    int dist;
    node* src;
    umap<node*, int> edges; // value = distance

    node(){ vis = false, dist = INF, src = NULL; } };

  //! Comment why we need keymap
  umap<T, node> nodes;
  umap<node*, T> keymap;

  graph(){}

  virtual void validate(){}

  // Does the graph contain the key as a node
  bool contains(T key){
    return nodes.find(key) != nodes.end(); }

  // Insert a new node representing the mappable data key
  void insert(T key){
    if(!contains(key))
      nodes[key] = node(), keymap[&nodes[key]] = key; }

  // Link two nodes together, creating two one-way edges
  void link(T key1, T key2, int dist){
    typename umap<T, node>::iterator it1, it2;
    assert((it1 = nodes.find(key1)) != nodes.end()
        && (it2 = nodes.find(key2)) != nodes.end(), "Bad graph link");
    node* node1 = &it1->second, *node2 = &it2->second;
    it1->second.edges[node2] = dist;
    it2->second.edges[node1] = dist; }

  // Reset graph for next path eval
  void reset(){
    typename umap<T, node>::iterator it;
    for(it = nodes.begin(); it != nodes.end(); ++it){
      node* n = &it->second;
      n->vis = false, n->dist = INF, n->src = NULL; } }

  // Find the shortest path between two given nodes
  vec<T> path(T start, T end){
    mmap<int, node*> pq;
    nodes[start].dist = 0;
    pq.insert({0, &nodes[start]});

    // Loop until priority queue is empty of nodes to visit
    while(!pq.empty()){
      mmap<int, node*>::iterator it = pq.begin();
      node* n = it->second;
      n->vis = true;
      pq.erase(it);
      umap<node*, int>::iterator it2;

      // Iterate each unvisited edge of node n
      for(it2 = n->edges.begin(); it2 != n->edges.end(); ++it2){
        node* n2 = it2->first;
        if(n2->vis) continue;

        // Update node with new distance, if shorter
        int dist = n->dist + it2->second;
        if(dist < n2->dist){
          n2->dist = dist;
          n2->src = n;
          pq.insert({it2->second, n2}); } } }

    // Generate path from start to end
    vec<T> rev, p;
    node* n = &nodes[end];
    while(n != &nodes[start]){
      rev.pb(keymap[n]);
      n = n->src; }
    rev.pb(start);
    for(int i = rev.size()-1; i >= 0; --i)
      p.pb(rev[i]);

    // Reset graph for later use and return
    reset();
    return p; } };

#endif

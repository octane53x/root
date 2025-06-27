// CUBE

#ifndef CUBE_HH
#define CUBE_HH

#include "obj.hh"

struct cube {

  static const int INNER = 8;
  static constexpr double INIT_SIZE = 1000000000000.0; // 10^12

  // Locations
  static const int LOCS = 6;
  static const uchar
      W = 1,
      E = 1 << 1,
      B = 1 << 2,
      T = 1 << 3,
      S = 1 << 4,
      N = 1 << 5;

  // Members
  int id;
  static int next_id;
  uchar loc;
  double size;
  point pos;

  int outer;
  umap<uchar, int> inner, adj;
  umap<int, cube> raw_cubes;
  umap<int, cube>* cubes;

  // Constructor (outermost cube)
  cube(){
    id = next_id++;
    loc = 0;
    size = INIT_SIZE;
    pos = point(0.0, 0.0, 0.0);
    outer = -1;
    cubes = &raw_cubes; }

  // Opposite location
  uchar opp(uchar loc){
    switch(loc){
    case W: return E;
    case E: return W;
    case B: return T;
    case T: return B;
    case S: return N;
    case N: return S;
    default:
      err("Called cube.opp on bad location");
      return 0; } }

  // Split the cube into 8 inner cubes
  void split(){
    vec<int> _inner;
    for(int i = 0; i < INNER; ++i){
      cube c;
      c.size = size / 2.0;
      c.outer = id;
      c.cubes = cubes;
      cubes->insert({c.id, c});
      _inner.pb(c.id); }

    // Set location and position
    int ii = 0;
    double x = pos.x;
    for(uchar i = W; i <= E; i <<= 1){
      double y = pos.y;
      for(uchar j = B; j <= T; j <<= 1){
        double z = pos.z;
        for(uchar k = S; k <= N; k <<= 1){
          cube* c = &(*cubes)[_inner[ii]];
          c->loc = i | j | k;
          c->pos = point(x, y, z);
          inner[c->loc] = c->id;
          z += size / 2.0;
          ++ii; }
        y += size / 2.0; }
      x += size / 2.0; }

    // Connect to other inner cubes
    for(int i = 0; i < INNER-1; ++i){
      cube* c1 = &(*cubes)[_inner[i]];
      for(int j = i+1; j < INNER; ++j){
        cube* c2 = &(*cubes)[_inner[j]];
        uchar overlap = c1->loc & c2->loc;
        uchar mask = 1;
        int n = 0;
        for(int k = 0; k < LOCS; ++k){
          if(overlap & mask) ++n;
          mask <<= 1; }
        if(n == 2){
          uchar d1 = c1->loc ^ overlap;
          c1->adj.insert({opp(d1), c2->id});
          uchar d2 = c2->loc ^ overlap;
          c2->adj.insert({opp(d2), c1->id}); } } }

    // Connect to adjacent
    for(int i = 0; i < INNER; ++i){
      cube* ci = &(*cubes)[_inner[i]];
      uchar mask = 1;
      for(int k = 0; k < LOCS; ++k){
        if(!(ci->loc & mask)) continue;
        if(adj.find(mask) == adj.end()) continue;
        cube* ca = &(*cubes)[adj[mask]];
        if(!ca->inner.empty()){
          uchar la = (ci->loc ^ mask) | opp(mask);
          ci->adj[mask] = (*cubes)[ca->inner[la]].id;
        }else ci->adj[mask] = ca->id;
        mask <<= 1;
      }
    }
  }
};

int cube::next_id = 1;

#endif

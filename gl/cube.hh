// CUBE

#ifndef CUBE_HH
#define CUBE_HH

#include "obj.hh"

struct cube {
  const static double INIT_SIZE = 1000000000000.0; // 10^12

  // Locations
  const static int LOCS = 6;
  const static uchar
      W = 1,
      E = 1 << 1,
      B = 1 << 2,
      T = 1 << 3,
      S = 1 << 4,
      N = 1 << 5;

  // Members
  bool outermost; //?
  uchar loc;
  double size;
  point pos;
  cube* outer;
  vec<cube*> inner;
  umap<uchar, cube*> adj;
  vec<cube> raw_cubes;
  vec<cube>* cubes;

  // Constructor (outermost cube)
  cube(){
    outermost = true;
    loc = 0;
    size = INIT_SIZE;
    pos = point(0.0, 0.0, 0.0);
    outer = NULL;
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
    default: return 0; } }

  // Split the cube into 8 inner cubes
  void split(){
    for(int i = 0; i < 8; ++i){
      cube c;
      c.outermost = false;
      c.size = size / 2.0;
      c.outer = this;
      c.cubes = cubes;
      cubes->pb(c);
      inner.pb(&cubes->back()); }

    // Set location and position
    int ii = 0;
    double x = pos.x;
    for(uchar i = W; i <= E; i <<= 1){
      double y = pos.y;
      for(uchar j = B; j <= T; j <<= 1){
        double z = pos.z;
        for(uchar k = S; k <= N; k <<= 1){
          inner[ii]->loc |= i | j | k;
          inner[ii]->pos = point(x, y, z);
          z += size / 2.0;
          ++ii; }
        y += size / 2.0; }
      x += size / 2.0; }

    // Connect to other inner cubes
    for(int i = 0; i < inner.size()-1; ++i){
      for(int j = i+1; j < inner.size(); ++j){
        uchar overlap = inner[i]->loc & inner[j]->loc;
        uchar mask = 1;
        int n = 0;
        for(int k = 0; k < LOCS; ++k){
          if(overlap & mask) ++n;
          mask <<= 1; }
        if(n == 2)
          inner[i]->adj.pb(inner[j]), inner[j]->adj.pb(inner[i]); } }

    // Connect to adjacent
    for(int i = 0; i < inner.size(); ++i){
      umap<uchar, cube*>::iterator it;
      for(it = adj.begin(); it != adj.end(); ++it){
        //!
      }
    }
  }
};

#endif

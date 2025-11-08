// CUBE

#ifndef CUBE_HH
#define CUBE_HH

#include "object.hh"

struct cube : thing {

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
  llu id;
  static llu next_id;
  uchar loc;
  double size;
  point pos;

  int outer;
  umap<uchar, llu> inner, adj;
  umap<llu, cube> raw_cubes;
  umap<llu, cube>* cubes;

  umap<llu, object*> objs;

  // Constructor (outermost cube)
  cube(){
    id = next_id++;
    loc = 0;
    size = INIT_SIZE;
    pos = point(0.0, 0.0, 0.0);
    outer = -1;
    cubes = &raw_cubes; }

  virtual void validate(){}

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
    vec<llu> _inner;
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
        for(int k = 0; k < LOCS; ++k, mask <<= 1)
          if(overlap & mask) ++n;
        if(n == 2){
          uchar d1 = c1->loc ^ overlap;
          c1->adj.insert({opp(d1), c2->id});
          uchar d2 = c2->loc ^ overlap;
          c2->adj.insert({opp(d2), c1->id}); } } }

    // Connect to adjacent cubes
    for(int i = 0; i < INNER; ++i){
      cube* ci = &(*cubes)[_inner[i]];
      uchar mask = 1;
      for(int k = 0; k < LOCS; ++k, mask <<= 1){
        if(!(ci->loc & mask)) continue;
        if(adj.find(mask) == adj.end()) continue;
        cube* ca = &(*cubes)[adj[mask]];
        if(!ca->inner.empty()){
          uchar la = (ci->loc ^ mask) | opp(mask);
          cube* cai = &(*cubes)[ca->inner[la]];
          ci->adj[mask] = cai->id;
          cai->adj[opp(mask)] = ci->id;
        }else ci->adj[mask] = ca->id; } } }

  // Merge a cube by deleting the inner cubes
  void merge(){
    umap<uchar, llu>::iterator it;
    for(it = inner.begin(); it != inner.end(); ++it){
      cube* ci = &(*cubes)[it->second];

      // Recursively delete further inner cubes
      ci->merge();

      // Link adjacent cubes to outer cube
      uchar mask = 1;
      for(int k = 0; k < LOCS; ++k, mask <<= 1){
        if(!(ci->loc & mask)) continue;
        if(ci->adj.find(mask) == ci->adj.end()) continue;
        cube* ca = &(*cubes)[ci->adj[mask]];
        if(ca->adj[opp(mask)] == ci->id)
          ca->adj[opp(mask)] = id; } // ci->outer

      // Remove inner cube from cube map
      cubes->erase(cubes->find(ci->id)); }
    inner.clear(); }

  // Add an object to the cube
  void insert(object* o){
    // Try to add to inner cube
    for(int idi : c->inner){
      cube* ci = &(*cubes)[idi];
      // Check boundaries of all 3 dimensions
      if(o->pos.x >= ci->pos.x
          && o->pos.x + o->mdl->box.x <= ci->pos.x + ci->size.x
          && o->pos.y >= ci->pos.y
          && o->pos.y + o->mdl->box.y <= ci->pos.y + ci->size.y
          && o->pos.z >= ci->pos.z
          && o->pos.z + o->mdl->box.z <= ci->pos.z + ci->size.z){
        ci->insert(o);
        return; } }

    // If cube is empty or already split, add object
    if(objs.empty() || !c->inner.empty()){
      objs[o->id] = o;
      return; }

    // Split if object can fit in an inner cube
    double x0 = pos.x, xf = pos.x + size / 2.0;
    for(uchar i = W; i <= E; i <<= 1){
      double y0 = pos.y, yf = pos.y + size / 2.0;
      for(uchar j = B; j <= T; j <<= 1){
        double z0 = pos.z, zf = pos.y + size / 2.0;
        for(uchar k = S; k <= N; k <<= 1){
          // Check boundaries of all 3 dimensions
          if(o->pos.x >= x0 && o->pos.x + o->mdl->box.x <= xf
              && o->pos.y >= y0 && o->pos.y + o->mdl->box.y <= yf
              && o->pos.x >= z0 && o->pos.z + o->mdl->box.z <= zf){
            split();
            insert(o);
            return; }
          z0 += size / 2.0, zf += size / 2.0; }
        y0 += size / 2.0, yf += size / 2.0; }
      x0 += size / 2.0, xf += size / 2.0; } }

  // Remove an object from the cube
  void remove(model* m){
    //!
  }

  // Move an object within the cube
  void move(model* m, point p, uvec rot){
    remove(m);
    insert(m, p, rot); }

  // Raytracer
  color get_pixel(point p, uvec u){
    //!
    return GREEN; } };

int cube::next_id = 1;

#endif

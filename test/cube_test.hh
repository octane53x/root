// CUBE TEST

#include "../gl/cube.hh"

namespace cube_test {

void test1(){
  const char* MSG = "Improper cube split";
  cube c;
  c.split();
  umap<uchar, int>::iterator it;
  for(it = c.inner.begin(); it != c.inner.end(); ++it){
    int id = it->second;
    cube* ci = &(*c.cubes)[id];
    assert(ci->adj.size() == 3, MSG); }

  uchar
    W = 1,
    E = 1 << 1,
    B = 1 << 2,
    T = 1 << 3,
    S = 1 << 4,
    N = 1 << 5;

  uchar
    ltnw = T | N | W,
    ltne = T | N | E,
    ltsw = T | S | W,
    ltse = T | S | E,
    lbnw = B | N | W,
    lbne = B | N | E,
    lbsw = B | S | W,
    lbse = B | S | E;

  cube
    *tnw = &(*c.cubes)[c.inner[ltnw]],
    *tne = &(*c.cubes)[c.inner[ltne]],
    *tsw = &(*c.cubes)[c.inner[ltsw]],
    *tse = &(*c.cubes)[c.inner[ltse]],
    *bnw = &(*c.cubes)[c.inner[lbnw]],
    *bne = &(*c.cubes)[c.inner[lbne]],
    *bsw = &(*c.cubes)[c.inner[lbsw]],
    *bse = &(*c.cubes)[c.inner[lbse]];

  // TNW
  assert(tnw->adj[B] == bnw->id, MSG);
  assert(tnw->adj[S] == tsw->id, MSG);
  assert(tnw->adj[E] == tne->id, MSG);

  // TNE
  assert(tne->adj[B] == bne->id, MSG);
  assert(tne->adj[S] == tse->id, MSG);
  assert(tne->adj[W] == tnw->id, MSG);

  // TSW
  assert(tsw->adj[B] == bsw->id, MSG);
  assert(tsw->adj[N] == tnw->id, MSG);
  assert(tsw->adj[E] == tse->id, MSG);

  // TSE
  assert(tse->adj[B] == bse->id, MSG);
  assert(tse->adj[N] == tne->id, MSG);
  assert(tse->adj[W] == tsw->id, MSG);

  // BNW
  assert(bnw->adj[T] == tnw->id, MSG);
  assert(bnw->adj[S] == bsw->id, MSG);
  assert(bnw->adj[E] == bne->id, MSG);

  // BNE
  assert(bne->adj[T] == tne->id, MSG);
  assert(bne->adj[S] == bse->id, MSG);
  assert(bne->adj[W] == bnw->id, MSG);

  // BSW
  assert(bsw->adj[T] == tsw->id, MSG);
  assert(bsw->adj[N] == bnw->id, MSG);
  assert(bsw->adj[E] == bse->id, MSG);

  // BSE
  assert(bse->adj[T] == tse->id, MSG);
  assert(bse->adj[N] == bne->id, MSG);
  assert(bse->adj[W] == bsw->id, MSG); }

void test_all(){
  printf("\nTesting cube:\n");
  test1();
  printf("All tests pass :)\n"); } }

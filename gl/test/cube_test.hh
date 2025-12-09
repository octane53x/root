// CUBE TEST

#include "../gl/obj.hh"
#include "../gl/cube.hh"

namespace cube_test {

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

void test1(){
  const char* MSG = "Improper cube split";
  cube c;
  c.split();
  umap<uchar, int>::iterator it;
  for(it = c.inner.begin(); it != c.inner.end(); ++it){
    int id = it->second;
    cube* ci = &(*c.cubes)[id];
    assert(ci->adj.size() == 3, func, MSG); }

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
  assert(tnw->adj[B] == bnw->id, func, MSG);
  assert(tnw->adj[S] == tsw->id, func, MSG);
  assert(tnw->adj[E] == tne->id, func, MSG);

  // TNE
  assert(tne->adj[B] == bne->id, func, MSG);
  assert(tne->adj[S] == tse->id, func, MSG);
  assert(tne->adj[W] == tnw->id, func, MSG);

  // TSW
  assert(tsw->adj[B] == bsw->id, func, MSG);
  assert(tsw->adj[N] == tnw->id, func, MSG);
  assert(tsw->adj[E] == tse->id, func, MSG);

  // TSE
  assert(tse->adj[B] == bse->id, func, MSG);
  assert(tse->adj[N] == tne->id, func, MSG);
  assert(tse->adj[W] == tsw->id, func, MSG);

  // BNW
  assert(bnw->adj[T] == tnw->id, func, MSG);
  assert(bnw->adj[S] == bsw->id, func, MSG);
  assert(bnw->adj[E] == bne->id, func, MSG);

  // BNE
  assert(bne->adj[T] == tne->id, func, MSG);
  assert(bne->adj[S] == bse->id, func, MSG);
  assert(bne->adj[W] == bnw->id, func, MSG);

  // BSW
  assert(bsw->adj[T] == tsw->id, func, MSG);
  assert(bsw->adj[N] == bnw->id, func, MSG);
  assert(bsw->adj[E] == bse->id, func, MSG);

  // BSE
  assert(bse->adj[T] == tse->id, func, MSG);
  assert(bse->adj[N] == bne->id, func, MSG);
  assert(bse->adj[W] == bsw->id, func, MSG); }

void test2(){
  const char* MSG = "Improper cube split";
  cube c;
  c.split();
  cube* tnw = &(*c.cubes)[c.inner[ltnw]];
  cube* tne = &(*c.cubes)[c.inner[ltne]];
  tnw->split();
  tne->split();

  cube* tnw_bne = &(*c.cubes)[tnw->inner[lbne]];
  cube* tne_bnw = &(*c.cubes)[tne->inner[lbnw]];
  cube* bnw = &(*c.cubes)[c.inner[lbnw]];
  assert(tnw_bne->adj[E] == tne_bnw->id, func, MSG);
  assert(tne_bnw->adj[W] == tnw_bne->id, func, MSG);
  assert(tnw_bne->adj[B] == bnw->id, func, MSG);
  assert(bnw->adj[T] == tnw->id, func, MSG);
  assert(tnw_bne->adj.find(N) == tnw_bne->adj.end(), func, MSG); }

void test3(){
  const char* MSG = "Improper cube merge";
  cube c;
  c.split();
  cube* tnw = &(*c.cubes)[c.inner[ltnw]];
  cube* tne = &(*c.cubes)[c.inner[ltne]];
  tnw->split();
  tne->split();
  tne->merge();

  cube* tnw_tne = &(*c.cubes)[tnw->inner[ltne]];
  assert(tne->inner.empty(), func, MSG);
  assert(c.cubes->size() == 16, func, MSG);
  assert(tnw_tne->adj[E] == tne->id, func, MSG); }

// void test4(){
//   const char* MSG = "Improper cube insert";
//   object o;
//   model m;
//   m.box = point(50.0, 50.0, 50.0);
//   o.mdl = m;
//   o.pos = point(

//   cube c;
//   c.size = 250.0;
// }

void test_all(){
  printf("\nTesting cube:\n");
  test1();
  test2();
  test3();
  //test4();
  printf("All tests pass :)\n"); } }

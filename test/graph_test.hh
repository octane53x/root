// GRAPH TEST

#include "../gl/graph.hh"

namespace graph_test {

void test1(){
  graph<int> g;
  for(int i = 0; i < 10; ++i)
    g.insert(i);

  g.link(0, 1, 1);
  g.link(1, 2, 1);
  g.link(1, 3, 1);
  g.link(2, 4, 1);
  g.link(3, 5, 1);
  g.link(4, 6, 1);
  g.link(4, 7, 1);
  g.link(5, 8, 1);
  g.link(5, 9, 1);

  vec<int> p = g.path(0, 9);
  assert(p.size() == 5 && p[0] == 0 && p[1] == 1 && p[2] == 3 && p[3] == 5
      && p[4] == 9, "Graph test 1 failed"); }

void test2(){
  graph<int> g;
  for(int i = 0; i < 6; ++i)
    g.insert(i);

  g.link(0, 1, 3);
  g.link(0, 2, 1);
  g.link(2, 3, 1);
  g.link(3, 4, 1);
  g.link(4, 5, 1);
  g.link(5, 1, 1);

  vec<int> p = g.path(0, 1);
  assert(p.size() == 2 && p[0] == 0 && p[1] == 1, "Graph test 2 failed"); }

void test3(){
  graph<int> g;
  for(int i = 0; i < 7; ++i)
    g.insert(i);

  g.link(0, 1, 2);
  g.link(1, 2, 2);
  g.link(2, 5, 2);
  g.link(5, 4, 2);
  g.link(1, 3, 2);
  g.link(3, 4, 2);
  g.link(4, 6, 2);
  g.link(0, 3, 5);
  g.link(3, 6, 5);

  vec<int> p = g.path(0, 6);
  assert(p.size() == 5 && p[0] == 0 && p[1] == 1 && p[2] == 3 && p[3] == 4
      && p[4] == 6, "Graph test 3 failed"); }

void test_all(){
  printf("\nTesting graph:\n");
  test1();
  test2();
  test3();
  printf("All tests pass :)\n"); } }

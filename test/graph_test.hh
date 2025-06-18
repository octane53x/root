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
  assert(p.size() == 4 && p[0] == 0 && p[1] == 3 && p[2] == 5 && p[3] == 9,
      "Graph test 1 failed"); }

void test2(){}

void test_all(){
  test1();
  test2();
} }

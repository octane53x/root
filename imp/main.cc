// MAIN

#include <stdio.h>
#include <stdlib.h>

#include <string>
#include <vector>
#include <map>

#define str string
#define vec vector
#define pb push_back

using namespace std;

struct Earth {
  map<str, int> minerals;
  Earth(){}
};

struct Planet {
  vec<vec<vec<Earth> > > earth;
  Planet(){}
};

const int DIM = 100;

const vec<str> MINERALS = {
  "IRON",
  "COPPER",
  "SILICON",
  "PLATINUM",
  "TITANIUM"
};

int main(){
  int i,j,k;
  Planet p;
  
  for(i = 0; i < DIM; ++i){
    p.earth.pb(vec<vec<Earth> >());
    for(j = 0; j < DIM; ++j){
      p.earth[i].pb(vec<Earth>());
      for(k = 0; k < DIM; ++k)
        p.earth[i][j].pb(Earth());
    }
  }

  for(i = 0; i < MINERALS.size(); ++i)
    p.earth[0][0][0].minerals[MINERALS[i]] = rand() % DIM + 1;

  printf("%d\n", p.earth[0][0][0].minerals.size());
}
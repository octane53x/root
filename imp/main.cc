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

struct Feature {
  int health;
  str name;
  Feature(){}
};

struct Unit {
  int health, speed;
  str name;
  Unit(){}
}

struct Tile {
  int altitude;
  str name;
  Feature* feature;
  vec<Unit> units;
  Tile(){}
};

struct Planet {
  vec<vec<vec<Earth> > > earth;
  vec<vec<Tile> > surface;
  vec<Feature> features;
  Planet(){}
};

const int C_MINERALS = 3;
const vec<str> MINERALS = {
  // CUSTOM:
  "SOIL",
  "SAND",
  "STONE",
  // RANDOM:
  "GRANITE",
  "MARBLE",
  "LIMESTONE",
  "SANDSTONE",
  "COAL",
  "IRON",
  "COPPER",
  "NICKEL",
  "GYPSUM",
  "TIN",
  "ALUMINUM",
  "PHOSPHORUS",
  "SILICON",
  "SILVER",
  "GOLD",
  "PLATINUM",
  "TITANIUM",
  "ADAMANTIUM",
  "URANIUM",
  "NEPTUNIUM",
  "PLUTONIUM"
};

const vec<str> GEMS = {
  "AMETHYST", // A
  "TOURMALINE", // B
  "JASPER", // C
  "DIAMOND", // D
  "EMERALD", // E
  "MALACHITE", // F
  "JADE", // G
  "AMAZONITE", // H
  "ZIRCON", // I
  "LAPIS LAZULI", // J
  "OBSIDIAN", // K
  "GASPEITE", // L
  "HEMATITE", // M
  "CHAROITE", // N
  "AMBER", // O
  "AQUAMARINE", // P
  "TURQUOISE", // Q
  "GARNET", // R
  "SAPPHIRE", // S
  "PEARL", // T
  "AZURITE", // U
  "RHODONITE", // V
  "QUARTZ", // W
  "ONYX", // X
  "TANZANITE", // Y
  "TOPAZ", // Z
  "OPAL", // 0
  "RUBY", // 1
  "CARNELIAN", // 2
  "AGATE", // 3
  "PERIDOT", // 4
  "CITRINE" // 5
};

const int
  MINE_WIDTH = 201,
  MINE_DEPTH = 100,
  MIN_MINERALS = 3,
  MAX_MINERALS = 10,
  MIN_BLOCKS = 10,
  MAX_BLOCKS = 10000;

template <typename T>
bool contains(vec<T>& v, T t){
  for(int i = 0; i < v.size(); ++i)
    if(v[i] == t) return true;
  return false;
}

void fill_earth(Planet& p){
  // Custom minerals
  for(int i = 0; i < MINE_WIDTH; ++i)
    for(int j = 0; j < MINE_WIDTH; ++j)
      for(int k = 0; k < MINE_DEPTH; ++k){
        p.earth[i][j][k].minerals["SOIL"] = 100;
        if(!(rand() % 4))
          p.earth[i][j][k].minerals["STONE"] = rand() % 100 + 1;
      }

  // Random minerals
  vec<str> used;
  int M = rand() % (MAX_MINERALS - MIN_MINERALS) + MIN_MINERALS;
  for(int m = 0; m < M; ++m){
    
    // Select mineral
    int r;
    bool done = false;
    while(!done){
      r = rand() % MINERALS.size();
      if(!contains(used, MINERALS[r]))
        done = true, used.pb(MINERALS[r]);
    }
    
    // Place mineral
    int B = //! resume
  }
}

int main(){
  int i,j,k, n;
  Planet p;
  
  fill_earth(p);
  
  n = 0;
  for(i = 0; i < MINE_WIDTH; ++i)
    for(j = 0; j < MINE_WIDTH; ++j)
      for(k = 0; k < MINE_DEPTH; ++k)
        n += p.earth[i][j][k].minerals["PLATINUM"];
  printf("Platinum: %d\n", n);
}
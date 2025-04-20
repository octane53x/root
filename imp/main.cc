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
  
  void fill_earth(){
    
  }
};

const int DIM = 100;

const vec<str> MINERALS = {
  "SOIL",
  "SAND",
  "STONE",
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
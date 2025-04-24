// MAIN

#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#include <string>
#include <vector>
#include <map>

#define ll long long
#define llu unsigned long long
#define str string
#define vec vector
#define pb push_back

using namespace std;

struct nat {
  vec<llu> data;
  
  nat(){ data.pb(0) }
  nat(ll n){ data.pb(abs(n)); }
  nat(const nat& n){ *this = n; }
  
  void validate(){
    if(data.empty()) data.pb(0);
  }
  
  nat& operator=(const nat& n){
    data.clear();
    data = n.data;
    validate();
  }
  
  bool operator==(const nat& n) const {
    for(int i = 0; i < data.size(); ++i)
      if(data[i] != n.data[i]) return false;
    return true;
  }
  
  bool operator<(const nat& n) const {
    if(data.size() < n.data.size()) return true;
    if(data.size() > n.data.size()) return false;
    for(int i = data.size()-1; i >= 0; --i){
      if(data[i] < n.data[i]) return true;
      if(data[i] > n.data[i]) return false;
    }
    return false;
  }
  
  bool operator<=(const nat& n) const {
    return (*this == n || *this < n);
  }
  bool operator>(const nat& n) const {
    return !(*this <= n);
  }
  bool operator>=(const nat& n) const {
    return !(*this < n);
  }
  
  nat& operator&=(const nat& n){
    if(n.data.size() < data.size())
      for(int i = 0; i < data.size()-n.data.size(); ++i)
        data.pop_back();
    for(int i = 0; i < data.size(); ++i)
      data[i] &= n.data[i];
    return *this;
  }
  
  nat& operator|=(const nat& n){
    if(n.data.size() < data.size())
      for(int i = 0; i < data.size()-n.data.size(); ++i)
        data.pop_back();
    for(int i = 0; i < data.size(); ++i)
      data[i] |= n.data[i];
    return *this;
  }
  
  nat& operator^=(const nat& n){
    if(n.data.size() < data.size())
      for(int i = 0; i < data.size()-n.data.size(); ++i)
        data.pop_back();
    for(int i = 0; i < data.size(); ++i)
      data[i] ^= n.data[i];
    return *this;
  }
  
  nat operator&(const nat& n) const {
    nat r = *this;
    r &= n;
    return r;
  }
  nat operator|(const nat& n) const {
    nat r = *this;
    r |= n;
    return r;
  }
  nat operator^(const nat& n) const {
    nat r = *this;
    r ^= n;
    return r;
  }
  
  nat& operator+=(const nat& n){
    
  }
};

struct num {
  bool neg;
  nat top, bot;
  
  num(): top(0), bot(1) {}
  num(ll n): neg(n < 0), top(abs(n)), bot(1) {}
  
  void validate(){
    if(bot == 0) bot = 1;
    nat g = top.gcd(bot);
    if(g > 1) top /= g, bot /= g;
  }
  
  num& operator=(const num& n){
    top = n.top, bot = n.bot, neg = n.neg;
    validate();
    return *this;
  }
  
  bool operator==(const num& n) const {
    return top == n.top && bot = n.bot && neg = n.neg;
  }
};

struct point {
  int x,y,z;
  
  point(){}
  point(int _x, int _y): x(_x), y(_y), z(0) {}
  point(int _x, int _y, int _z): x(_x), y(_y), z(_z) {}
  
  double dist(point& p){
    int a = abs(x - p.x), b = abs(y - p.y), c = abs(z - p.z);
    return sqrt(a*a + b*b + c*c);
  }
};

template <typename T>
struct graph {
  vec<T> nodes;
  map<int, int> edges;
  graph(){}
};

struct Earth {
  map<str, int> minerals;
  vec<Earth*> tunnels;
  Earth(){}
};

struct Unit {
  int health, speed, power;
  str name;
  Unit(){}
};

struct Worker : Unit {
  Worker(){}
};

struct Feature {
  int health;
  str name;
  point loc;
  vec<Worker> workers;
  map<str, int> gems;
  Feature(){}
};

struct Tower : Feature {
  int damage, range;
  graph<point> border;
  Tower(){}
};

struct Mine : Feature {
  Earth* top;
  Mine(){}
};

struct Recipe {
  map<str, int> parts, product;
  Recipe(){}
};

struct Factory : Feature {
  Recipe recipe;
  Factory(){}
};

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
// MAIN

#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <math.h>

#include <string>
#include <vector>
#include <map>

#define ll long long
#define llu unsigned long long
#define str string
#define vec vector
#define pb push_back

#define SHIFT_MAX (1 << 30)

using namespace std;

void pass(){}

void err(char* msg){
  printf("ERR: %s\n", msg);
  exit(-1);
}

void assert(bool b, char* msg){
  if(!b) err((str("ASSERT: ") + str(msg)).c_str());
}

void sleep(int ms){
  clock_t start = clock();
  while(1){
    clock_t t = clock();
    int p = (double)(t - start) / CLOCKS_PER_SEC * 1000.0;
    if(p >= ms) break;
  }
}

int min(int x, int y){ return (x < y) ? x : y; }
int max(int x, int y){ return (x > y) ? x : y; }

template <typename T>
bool contains(vec<T>& v, T t){
  for(int i = 0; i < v.size(); ++i)
    if(v[i] == t) return true;
  return false;
}

struct nat {
  vec<llu> data;
  
  nat(){ data.pb(0) }
  nat(int n){ data.pb(abs(n)); }
  nat(const nat& n){ *this = n; }
  
  void validate(){
    while(data[data.size()-1] == 0)
      data.pop_back();
    if(data.empty()) data.pb(0);
  }
  
  int to_int(){
    assert(*this <= (int)((1LL << 31) - 1), "Number too large to convert");
    return data[0];
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
    validate();
    return *this;
  }
  
  nat& operator|=(const nat& n){
    if(n.data.size() < data.size())
      for(int i = 0; i < data.size()-n.data.size(); ++i)
        data.pop_back();
    for(int i = 0; i < data.size(); ++i)
      data[i] |= n.data[i];
    validate();
    return *this;
  }
  
  nat& operator^=(const nat& n){
    if(n.data.size() < data.size())
      for(int i = 0; i < data.size()-n.data.size(); ++i)
        data.pop_back();
    for(int i = 0; i < data.size(); ++i)
      data[i] ^= n.data[i];
    validate();
    return *this;
  }
  
  nat& operator>>=(const nat& n){
    if(n > SHIFT_MAX){
      *this = 0;
      return *this;
    }
    int sh = n.to_int();
    int S = sh / 64, s = sh % 64;
    for(int i = 0; i < data.size()-S; ++i)
      data[i] = data[i+S];
    for(int i = 0; i < S; ++i)
      data.pop_back();
    for(int i = 0; i < data.size()-1; ++i)
      data[i] = ((data[i] >> s) | (data[i+1] << (64-s)));
    data[data.size()-1] >>= s;
    validate();
    return *this;
  }
  
  nat& operator<<=(const nat& n){
    assert(n < SHIFT_MAX, "Shift too large");
    int sh = n.to_int();
    int S = sh / 64, s = sh % 64, k = data.size()-1;
    for(int i = 0; i < S; ++i)
      data.pb(0);
    for(int i = k; i >= 0; --i)
      data[i+S] = data[i];
    data.pb(0);
    for(int i = data.size()-1; i > 0; --i)
      data[i] = ((data[i] << s) | (data[i-1] >> (64-s)));
    data[0] <<= s;
    validate();
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
  nat operator>>(const nat& n) const {
    nat r = *this;
    r >>= n;
    return r;
  }
  nat operator<<(const nat& n) const {
    nat r = *this;
    r <<= n;
    return r;
  }
  
  nat& operator+=(const nat& n){
    if(n.data.size() > data.size())
      for(int i = 0; i < n.data.size()-data.size(); ++i)
        data.pb(0);
    bool carry = false;
    for(int i = 0; i < data.size(); ++i){
      llu b = 1;
      for(int j = 0; j < 64; ++j){
        if(data[i] | b){
          if(n.data[i] | b){
            if(carry) pass();
            else data[i] &= (~b), carry = true;
          }else{
            if(carry) data[i] &= (~b);
            else pass();
          }
        }else{
          if(n.data[i] | b){
            if(carry) pass();
            else data[i] |= b, carry = true;
          }else{
            if(carry) data[i] |= b, carry = false;
            else pass();
          }
        }
        b <<= 1;
      }
    }
    if(carry)
      data.pb(1);
    validate();
    return *this;
  }
  
  nat kar(const nat& x, const nat& y){
    if(x.data.size() > 1 && y.data.size() > 1){
      int N = (min(x.data.size(), y.data.size()) >> 1);
      nat xl,xr,yl,yr;
      xr.data.clear(), xl.data.clear(), yl.data.clear(), yr.data.clear();
      for(int i = 0; i < N; ++i)
        xr.pb(x[i]);
      for(int i = N; i < x.data.size(); ++i)
        xl.pb(x[i]);
      for(int i = 0; i < N; ++i)
        yr.pb(y[i]);
      for(int i = N; i < y.data.size(); ++i)
        yl.pb(y[i]);
      return (kar(xl, yl) << (N << 7))
           + ((kar(xl + xr, yl + yr) - kar(xl, yl) - kar(xr, yr)) << (N << 6))
           + kar(xr, yr);
    }
    
  }
  
  nat& operator*=(const nat& n){
    nat r = kar(*this, n);
    *this = r;
    validate();
    return *this;
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

struct Block {
  map<str, int> minerals;
  vec<Block*> tunnels;
  Block(){}
};

struct Unit {
  int health, speed, energy;
  str name;
  Unit(){}
};

struct Worker : Unit {
  Worker(){}
};

struct Bot : Unit {
  map<str, int> gems;
};

struct Feature {
  int health;
  str name;
  point loc;
  Feature(){}
};

struct Tree : Feature {
  int wood;
  Tree(): name("TREE") {}
};

struct Tower : Feature {
  int damage, range;
  graph<point> border;
  map<str, int> gems;
  Tower(): name("TOWER") {}
};

struct Mine : Feature {
  Block* top;
  vec<Worker> workers;
  map<str, int> gems;
  Mine(): name("MINE") {}
};

struct Farm : Feature {
  graph<point> border;
  vec<vec<str> > crops;
  vec<Worker> workers;
  map<str, int> gems;
  Farm(): name("FARM") {}
};

struct Recipe {
  map<str, int> parts, product;
  Recipe(){}
};

struct Factory : Feature {
  Recipe recipe;
  map<str, int> gems;
  Factory(): name("FACTORY") {}
};

struct Tile {
  int altitude;
  str name;
  Feature* feature;
  vec<Unit> units;
  Tile(){}
};

struct Planet {
  vec<vec<vec<Block> > > earth;
  vec<vec<Tile> > surface;
  vec<Feature> features;
  Planet(){}
};

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
    //! int B =
  }
}

void move(int x, int y){
  
}

void console(){
  printf("(1) MOVE\n");
  printf("(2) BUILD\n");
  printf("(3) RECIPE\n");
  int c;
  scanf("%d", c);
  switch(c){
  case 1:
    printf("LOC: %d, %d\n");
    printf("Enter x y\n");
    int x,y;
    scanf("%d %d", x, y);
    move(x, y);
    break;
    
  case 2:
    
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
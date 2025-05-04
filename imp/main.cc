// MAIN

#include "../lang/core/incl.hh"

struct Game;
Game g;

struct Block {
  map<str, int> minerals;
  vec<Block*> tunnels;
  Block(){} };

struct Unit {
  int id, health, speed, energy;
  str name;
  point loc;
  Unit(){
    id = g.next_id(); } };

struct Worker : Unit {
  int progress;
  Worker(){} };

struct Bot : Unit {
  map<str, int> gems; };

struct Feature {
  int id, health;
  str name;
  point loc, size;
  Feature(){
    id = g.next_id(); } };

struct Tree : Feature {
  int wood;
  Tree(): name("TREE") {} };

struct Tower : Feature {
  int damage, range;
  graph<point> border;
  map<str, int> gems;
  Tower(int _x, int _y): name("TOWER"), loc(point(_x, _y)),
                         size(point(TOWER_SIZE, TOWER_SIZE)) {} };

struct Mine : Feature {
  Block* top;
  vec<Worker> workers;
  map<str, int> gems;
  Mine(int _x, int _y): name("MINE"), loc(point(_x, _y)),
                        size(point(MINE_SIZE, MINE_SIZE)) {} };

struct Farm : Feature {
  graph<point> border;
  vec<vec<str> > crops;
  vec<Worker> workers;
  map<str, int> gems;
  Farm(): name("FARM") {} };

struct Recipe {
  map<str, int> parts, product;
  Recipe(){} };

struct Factory : Feature {
  Recipe recipe;
  map<str, int> gems;
  Factory(int _x, int _y): name("FACTORY"), loc(point(_x, _y)),
                           size(point(FACTORY_SIZE, FACTORY_SIZE)) {} };

struct Lab : Feature {
  map<str, int> gems;
  Lab(): name("LAB") {} };

struct Tile {
  int altitude;
  str name;
  Feature* feature;
  vec<Unit*> units;
  Tile(){} };

struct Planet {
  int id;
  vec<vec<vec<Block> > > earth;
  vec<vec<Tile> > surface;
  vec<Feature*> features;
  Planet(){
    id = g.next_id();
    gen_earth();
    gen_surface(); }
  void gen_earth();
  void gen_surface(); };

struct Game {
  int last_id;
  Planet* planet; // Where the player is
  Bot* player;
  Game(): last_id(1) {
    planet = new Planet();
    player = new Bot();
    int x = PLAYER_START_X, y = PLAYER_START_Y;
    planet->surface[x][y].units.pb(player);
    player->loc.x = x, player->loc.y = y; }
  int next_id(){
    return ++last_id; }
  // Player actions
  void move(int x, int y);
  void build(Feature f); };

void Planet::gen_earth(){
  // Custom minerals
  for(int i = 0; i < MINE_WIDTH; ++i)
    for(int j = 0; j < MINE_WIDTH; ++j)
      for(int k = 0; k < MINE_DEPTH; ++k){
        earth[i][j][k].minerals["SOIL"] = 100;
        if(!(rand() % 4))
          earth[i][j][k].minerals["STONE"] = rand() % 100 + 1; }

  // Random minerals
  vec<str> used;
  int M = rand() % (MAX_MINERALS - MIN_MINERALS) + MIN_MINERALS;
  for(int m = 0; m < M; ++m){

    // Select mineral
    int r;
    bool done = false;
    while(!done){
      r = rand() % (MINERALS.size()-C_MINERALS) + C_MINERALS;
      if(!contains(used, MINERALS[r]))
        done = true, used.pb(MINERALS[r]); }

    // Place mineral
    //! int B =
  } }

void Planet::gen_surface(){
  for(int i = 0; i < SURFACE_WIDTH; ++i){
    surface.pb(vec<Tile>());
    for(int j = 0; j < SURFACE_WIDTH; ++j){
      Tile t;
      t.altitude = 0;
      t.name = "GRASSLAND";
      surface[i].pb(t); } } }

void Game::move(int x, int y){
  if(planet->surface[x][y].feature != NULL){
    printf("ERR: Tile occupied\n");
    return; }
  sleep(1000);
  const vec<Unit>& u = planet->surface[xp][yp].units;
  int xp = player->loc.x, yp = player->loc.y;
  for(int i = 0; i < u.size(); ++i){
    if(u[i].id = player->id){
      Bot b = (Bot)u[i];
      u.erase(u.begin() + i);
      u.pb(b);
      break; } }
  printf("Moved player from (%d,%d) to (%d,%d)\n", xp,yp,x,y); }

void Game::build(Feature* f){
  for(int x = f->x; x < f->x+f->size.x; ++x)
    for(int y = f->y; y < f->y+f->size.y; ++y)
      if(planet->surface[x][y].feature != NULL){
        printf("ERR: Tile occupied\n");
        return; }
  planet->features.pb(f);
  for(int x = f->x; x < f->x+f->size.x; ++x)
    for(int y = f->y; y < f->y+f->size.y; ++y)
      planet->surface[x][y].feature = f; }

void console(){
  printf("(1) MOVE\n");
  printf("(2) BUILD\n");
  //printf("(3) RECIPE\n");
  int c;
  scanf("%d", c);
  switch(c){
  case 1:
    printf("LOC: (%d,%d)\n", g.player->loc.x, g.player->loc.y);
    printf("Enter x y\n");
    int x,y;
    scanf("%d %d", x, y);
    move(x, y);
    break;

  case 2:
    printf("(1) TOWER\n");
    printf("(2) MINE\n");
    printf("(3) FACTORY\n");
    scanf("%d", c);
    printf("Enter x y\n");
    int x,y;
    scanf("%d %d", x, y);
    Planet* p = g.planet;
    switch(c){
    case 1:
      build(new Tower(x, y));
      break;
    case 2:
      build(new Mine(x, y));
      break;
    case 3:
      build(new Factory(x, y));
      break;
    default:
      break; }
    break;

  case 3:

    break;

  default:

    break; } }

void test(){}

int main(){
  g.p->gen_earth();
  g.p->gen_surface();
  //while(1) console();
  test(); }

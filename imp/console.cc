// IMP CONSOLE

#include "impact.hh"

void console(Game& g){
  printf("(1) MOVE\n");
  printf("(2) BUILD\n");
  //printf("(3) RECIPE\n");
  int c;
  scanf("%d", &c);
  int x,y;
  switch(c){
  case 1:
    printf("LOC: (%d,%d)\n", g.player->loc.x, g.player->loc.y);
    printf("Enter x y\n");
    scanf("%d %d", &x, &y);
    g.move(x, y);
    break;

  case 2:
    printf("(1) TOWER\n");
    printf("(2) MINE\n");
    printf("(3) FACTORY\n");
    scanf("%d", &c);
    printf("Enter x y\n");
    scanf("%d %d", &x, &y);
    switch(c){
    case 1:
      g.build(new Tower(x, y));
      break;
    case 2:
      g.build(new Mine(x, y));
      break;
    case 3:
      g.build(new Factory(x, y));
      break;
    default:
      break; }
    break;

  //case 3: break;
  default:
    printf("default\n");
    break; } }

int main(){
  Game g;
  g.planet = new Planet();
  g.player = new Bot();
  int x = (int)floor(PLAYER_START.x), y = (int)floor(PLAYER_START.y);
  g.planet->surface[x][y].units.pb(g.player);
  g.player->loc = point(x, y);

  while(1) console(g);
  return 0;
}

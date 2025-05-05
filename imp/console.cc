// IMP CONSOLE

#include "imp.hh"

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

int main(){
  while(1) console();
  return 0;
}

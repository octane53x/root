// P2209

#include <stdio.h>

#define gc getchar_unlocked

int getn(){
  int n = 0; char c = gc(), f = 1;
  while(c != '-' && (c < '0' || c > '9')) c = gc();
  if(c == '-') f = -1, c = gc();
  while(c >= '0' && c <= '9') n = (n<<3) + (n<<1) + c - '0', c = gc();
  return n * f;
}

int main(){
  int T,X,Y, i,e,o;

  T = getn();
  while(T--){
    X = getn(), Y = getn();
    e = o = 0;
    for(i = X; i <= Y; ++i){
      if(i % X == 0){
        if(i & 1) o += i;
        else e += i;
      }
    }
    if(e >= o) printf("YES\n");
    else printf("NO\n");
  }
  return 0;
}

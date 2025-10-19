// TOURIST

#include <stdio.h>

#define gc getchar_unlocked

int getn(){
  int n = 0; char c = gc(), f = 1;
  while(c != '-' && (c < '0' || c > '9')) c = gc();
  if(c == '-') f = -1, c = gc();
  while(c >= '0' && c <= '9') n = (n<<3) + (n<<1) + c - '0', c = gc();
  return n * f;
}

int abs(int n){
  return (n < 0) ? -n : n;
}

int main(){
  int T,N,A,B,X,Y, i, d,md,mx,my;

  T = getn();
  while(T--){
    N = getn(), A = getn(), B = getn();
    md = 10000;
    for(i = 0; i < N; ++i){
      X = getn(), Y = getn();

      d = abs(A - X) + abs(B - Y);
      if(d < md)
        md = d, mx = X, my = Y;
    }

    printf("%d\n", md);
  }
  return 0;
}

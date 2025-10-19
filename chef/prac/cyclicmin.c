// CYCLICMIN

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
  int T,N, i;

  T = getn();
  while(T--){
    N = getn();

    for(i = 1; i <= N; i += 2)
      printf("%d ", i);
    for(i = 2; i <= N; i += 2)
      printf("%d ", i);
    printf("\n");
  }
  return 0;
}

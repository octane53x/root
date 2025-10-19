// MAKESUB

#include <stdio.h>

#define gc getchar_unlocked

int getn(){
  int n = 0; char c = gc(), f = 1;
  while(c != '-' && (c < '0' || c > '9')) c = gc();
  if(c == '-') f = -1, c = gc();
  while(c >= '0' && c <= '9') n = (n<<3) + (n<<1) + c - '0', c = gc();
  return n * f;
}

char S[101];
int main(){
  int T,N, i, a,b,n;

  T = getn();
  while(T--){
    N = getn();
    scanf("%s", S);

    a = 0;
    for(i = 0; i < N; ++i){
      if(S[i] == '0'){
        ++a;
        continue;
      }else break;
    }
    b = N-1;
    for(i = N-1; i >= 0; --i){
      if(S[i] == '0'){
        --b;
        continue;
      }else break;
    }

    n = 0;
    for(i = a; i <= b; ++i)
      if(S[i] == '0') ++n;
    printf("%d\n", n);
  }
  return 0;
}

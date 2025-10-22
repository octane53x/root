// P3209

#include <stdio.h>

#define gc getchar_unlocked

int getn(){
  int n = 0; char c = gc(), f = 1;
  while(c != '-' && (c < '0' || c > '9')) c = gc();
  if(c == '-') f = -1, c = gc();
  while(c >= '0' && c <= '9') n = (n<<3) + (n<<1) + c - '0', c = gc();
  return n * f;
}

int gcd(int a, int b){ return b ? gcd(b,a%b) : a; }

char lt(int a, int b, int n){
  if(gcd(a, n) > gcd(b, n)) return 1;
  if(gcd(a, n) < gcd(b, n)) return 0;
  if(a < b) return 1;
  return 0;
}

int N;

void sort(int* a, int n, char (*lt)()){
  if(n < 2) return;
  int p,t,*l,*r;
  p = a[n>>1], l = a, r = a+n-1;
  while(l <= r){
    if(lt(*l, p, N)){ l++; continue; }
    if(lt(p, *r, N)){ r--; continue; }
    t = *l; *l++ = *r; *r-- = t;
  }
  sort(a, r-a+1, lt), sort(l, a+n-l, lt);
}

int main(){
  int T, i, a[100];

  T = getn();
  while(T--){
    N = getn();
    for(i = 0; i < N; ++i)
      a[i] = i+1;
    sort(a, N, lt);
    for(i = 0; i < N; ++i)
      printf("%d ", a[i]);
    printf("\n");
  }
  return 0;
}

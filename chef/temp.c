//

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
  int T;

  T = getn();
  while(T--){

  }
  return 0;
}

// METHODS

#define pc putchar_unlocked
char pb[10];
void putn(int n){
  if(!n) pc('0');
  if(n < 0){ pc('-'); n *= -1; }
  int pi = 0;
  while(n) pb[pi++] = (n%10) + '0', n /= 10;
  while(pi) pc(pb[--pi]);
}

int abs(int a){ return (a < 0) ? -a : a; }
int min(int a, int b){ return (a < b) ? a : b; }
int max(int a, int b){ return (a > b) ? a : b; }

int gcd(int a, int b){ return b ? gcd(b,a%b) : a; }
int lcm(int a, int b){ return a / gcd(a,b) * b; }

int madd(int a, int b){ return ((a += b) >= MOD) ? a-MOD : a; }
int msub(int a, int b){ return ((a -= b) < 0) ? a+MOD : a; }
int mmul(int a, int b){ return (int)((ll)a * b % MOD); }

int gcde(int a, int b, int* x, int* y){
  if(!a){ *x = 0, *y = 1; return b; }
  int g,x1,y1;
  g = gcde(b % a, a, &x1, &y1), *x = y1 - (b / a) * x1, *y = x1;
  return g;
}
int minv(int a){
  int x,y;
  if(gcde(a, MOD, &x, &y) != 1) a /= 0;
  return (x % MOD + MOD) % MOD;
}
int mdiv(int a, int b){
  int inv = minv(b);
  return mmul(inv, a);
}

int mpow(int n, int e){
  int r = 1;
  while(e){
    if(e & 1) r = mmul(r, n);
    n = mmul(n, n), e >>= 1;
  }
  return r;
}

int nck[2001][2001]={{0}};
nck[0][0] = 1;
for(i = 1; i <= 2000; ++i){
  nck[i][0] = 1;
  for(j = 1; j <= i; ++j)
    nck[i][j] = madd(nck[i-1][j-1], nck[i-1][j]);
}

void sort(int* a, int n, char (*lt)()){
  if(n < 2) return;
  int p,t,*l,*r;
  p = a[n>>1], l = a, r = a+n-1;
  while(l <= r){
    if(lt(*l, p)){ l++; continue; }
    if(lt(p, *r)){ r--; continue; }
    t = *l; *l++ = *r; *r-- = t;
  }
  sort(a, r-a+1, lt), sort(l, a+n-l, lt);
}

void sort(int* a, int n){
  if(n < 2) return;
  int p,t,*l,*r;
  p = a[n>>1], l = a, r = a+n-1;
  while(l <= r){
    if(*l < p){ l++; continue; }
    if(*r > p){ r--; continue; }
    t = *l; *l++ = *r; *r-- = t;
  }
  sort(a, r-a+1), sort(l, a+n-l);
}

// BIT
int N, li[20],ln,an;
ll a[2000000];

void build(){
  int i,j,s;
  li[0] = 0, s = N, ln = 1;
  while(s > 1){
    li[ln] = li[ln-1] + s;
    for(i = li[ln-1], j = li[ln]; i < li[ln]-1; i += 2, j++)
      a[j] = a[i] + a[i+1]; //!
    s >>= 1, ++ln;
  }
  an = j;
}

void update(int i, int v){
  int l;
  l = 1, a[i] += v; //!
  while(l < ln) a[(i >>= 1) + li[l++]] += v; //!
}

ll query(int L, int R){
  int i,j,l,e,ia,ib;
  ll r;
  r = l = 0, e = 1, i = L;
  while(L <= R){
    if(i != an-1 && i != li[l+1]-1){
      j = li[l+1] + (L >> l+1);
      ia = (j - li[l+1]) << (l+1), ib = ia + (e << 1) - 1;
      if(ia == L && ib <= R){ i = j, e <<= 1, l++; continue; }
    }
    ia = (i - li[l]) << l;
    if(ia == L){
      r += a[i]; //!
      L += e;
      if(L > R) break;
    }
    if(R - L + 1 < e) e >>= 1, l--, i = li[l] + (ia >> l) + 1;
    else i++;
  }
  return r;
}

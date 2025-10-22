// P5209

#include <stdio.h>

#define gc getchar_unlocked

int getn(){
  int n = 0; char c = gc(), f = 1;
  while(c != '-' && (c < '0' || c > '9')) c = gc();
  if(c == '-') f = -1, c = gc();
  while(c >= '0' && c <= '9') n = (n<<3) + (n<<1) + c - '0', c = gc();
  return n * f;
}

int abs(int n){ return (n < 0) ? -n : n; }

char s[200001];
int a[200000], b[200000], d[200000], e[200000];
int main(){
  char w,t;
  int T,N, i,j,k, n,n1,n2;

  T = getn();
  while(T--){
    N = getn();
    scanf("%s", &s);

    n1 = n2 = 0;
    for(i = 1; i < N; ++i){
      if(s[i-1] == '0' && s[i] == '1') ++n1;
      if(s[i-1] == '1' && s[i] == '0') ++n2;
    }
    if(n1 == 0 || n2 == 0){
      printf("Bob\n");
      continue;
    }
    if(n1 == n2){
      printf("Alice\n");
      continue;
    }

    a[0] = n1 - n2;
    for(i = 1; i < N; ++i){
      if(s[i-1] == '0' && s[i] == '1') a[i] = a[i-1] - 1;
      else if(s[i-1] == '1' && s[i] == '0') a[i] = a[i-1] + 1;
      else a[i] = a[i-1];
    }
    b[N-1] = n1 - n2;
    for(i = N-2; i >= 0; --i){
      if(s[i] == '0' && s[i+1] == '1') b[i] = b[i+1] - 1;
      else if(s[i] == '1' && s[i+1] == '0') b[i] = b[i+1] + 1;
      else b[i] = b[i+1];
    }

    for(i = 0; i < N; ++i)
      d[i] = e[i] = -1;
    k = -1;
    for(i = 1; i < N; ++i){
      if(abs(a[i]) < abs(a[i-1])){
        for(j = i-1, n = 0; j > k; --j, ++n)
          d[j] = n;
        k = i-1;
      }
    }
    k = N;
    for(i = N-2; i >= 0; --i){
      if(abs(b[i]) < abs(b[i+1])){
        for(j = i+1, n = 0; j < k; ++j, ++n)
          e[j] = n;
        k = i+1;
      }
    }

    w = 0, t = 1, n = abs(n1 - n2);
    i = 0, j = N-1;
    while(i <= j){
      if(t == 1){
        if(d[i] <= e[j]){
          if(d[i] == 0) --n;
          ++i;
        }else{
          if(e[j] == 0) --n;
          --j;
        }
        if(n == 0){
          w = 1;
          break;
        }
      }else{
        if(d[i] >= e[j]) ++i;
        else --j;
      }
      t = (t == 1) ? 0 : 1;
    }

    if(w) printf("Alice\n");
    else printf("Bob\n");
  }
  return 0;
}

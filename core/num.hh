// NUM

//! Division is wrong

#ifndef NUM_HH
#define NUM_HH

#include "util.hh"

const int
    SHIFT_MAX = (1 << 30),
    BITS_PER_BLOCK = 64;

struct nat : thing {
  vec<llu> data;

  nat(){ data.pb(0); }
  nat(llu n){ data.pb(n); }
  nat(const nat& n){ *this = n; }

  virtual void validate(){
    while(data[data.size()-1] == 0)
      data.popb();
    if(data.empty()) data.pb(0); }

  int to_int() const {
    assert(*this <= INT_MAX, "Number too large to convert");
    return data[0]; }

  void print_bits() const {
    llu b = 1;
    b <<= BITS_PER_BLOCK - 1;
    while(b){
      if(data.back() & b) break;
      b >>= 1; }
    while(b){
      printf("%d", (data.back() & b) ? 1 : 0);
      b >>= 1; }
    for(int i = data.size()-2; i >= 0; --i){
      printf(",");
      b = 1, b <<= BITS_PER_BLOCK - 1;
      while(b){
        printf("%d", (data[i] & b) ? 1 : 0);
        b >>= 1; } } }

  void print() const {
    nat t = *this;
    vec<int> digits;
    while(t != 0){
      nat dn = t % 10;
      // printf("\n");
      // dn.print_bits();
      // printf("\n");
      int d = dn.to_int();
      digits.pb(d);
      t /= 10; }
    for(int i = digits.size()-1; i >= 0; --i)
      printf("%d", digits[i]); }

  nat& operator=(const nat& n){
    data.clear();
    data = n.data;
    validate();
    return *this; }

  bool operator==(const nat& n) const {
    for(int i = 0; i < data.size(); ++i)
      if(data[i] != n.data[i]) return false;
    return true; }
  bool operator!=(const nat& n) const {
    return !(*this == n); }

  bool operator<(const nat& n) const {
    if(data.size() < n.data.size()) return true;
    if(data.size() > n.data.size()) return false;
    for(int i = data.size()-1; i >= 0; --i){
      if(data[i] < n.data[i]) return true;
      if(data[i] > n.data[i]) return false; }
    return false; }
  bool operator<=(const nat& n) const {
    return (*this == n || *this < n); }
  bool operator>(const nat& n) const {
    return !(*this <= n); }
  bool operator>=(const nat& n) const {
    return !(*this < n); }

  nat& operator&=(const nat& n){
    if(n.data.size() < data.size())
      for(int i = 0; i < data.size()-n.data.size(); ++i)
        data.popb();
    for(int i = 0; i < data.size(); ++i)
      data[i] &= n.data[i];
    validate();
    return *this; }

  nat& operator|=(const nat& n){
    if(n.data.size() < data.size())
      for(int i = 0; i < data.size()-n.data.size(); ++i)
        data.popb();
    for(int i = 0; i < data.size(); ++i)
      data[i] |= n.data[i];
    validate();
    return *this; }

  nat& operator^=(const nat& n){
    if(n.data.size() < data.size())
      for(int i = 0; i < data.size()-n.data.size(); ++i)
        data.popb();
    for(int i = 0; i < data.size(); ++i)
      data[i] ^= n.data[i];
    validate();
    return *this; }

  nat& operator>>=(const nat& n){
    if(n > SHIFT_MAX){
      *this = 0;
      return *this; }
    int sh = n.to_int();
    int S = sh / BITS_PER_BLOCK, s = sh % BITS_PER_BLOCK;
    vec<llu> data2;
    for(int i = 0; i < data.size()-S; ++i)
      data2.pb(0);
    for(int i = 0; i < data2.size(); ++i){
      data2[i] |= data[i+S] >> s;
      if(s > 0 && i+S+1 < data.size())
        data2[i] |= data[i+S+1] << (BITS_PER_BLOCK-s); }
    data = data2;
    validate();
    return *this; }

  nat& operator<<=(const nat& n){
    assert(n < SHIFT_MAX, "Shift too large");
    int sh = n.to_int();
    int S = sh / BITS_PER_BLOCK, s = sh % BITS_PER_BLOCK;
    vec<llu> data2;
    for(int i = 0; i < data.size()+S+1; ++i)
      data2.pb(0);
    for(int i = data.size()-1; i >= 0; --i){
      if(s > 0) data2[i+S+1] |= data[i] >> (BITS_PER_BLOCK-s);
      data2[i+S] |= data[i] << s; }
    data = data2;
    validate();
    return *this; }

  nat operator&(const nat& n) const {
    nat r = *this;
    r &= n;
    return r; }
  nat operator|(const nat& n) const {
    nat r = *this;
    r |= n;
    return r; }
  nat operator^(const nat& n) const {
    nat r = *this;
    r ^= n;
    return r; }
  nat operator>>(const nat& n) const {
    nat r = *this;
    r >>= n;
    return r; }
  nat operator<<(const nat& n) const {
    nat r = *this;
    r <<= n;
    return r; }

  nat& operator+=(const nat& n){
    while(data.size() < n.data.size())
      data.pb(0);
    bool carry = false;
    for(int i = 0; i < data.size(); ++i){
      llu b = 1;
      for(int j = 0; j < BITS_PER_BLOCK; ++j, b <<= 1){
        if(data[i] & b){
          if(n.data[i] & b){
            if(carry) pass();
            else data[i] &= ~b, carry = true;
          }else{
            if(carry) data[i] &= ~b;
            else pass(); }

        }else{
          if(n.data[i] & b){
            if(carry) pass();
            else data[i] |= b;
          }else{
            if(carry) data[i] |= b, carry = false;
            else pass(); } } } }
    if(carry)
      data.pb(1);
    validate();
    return *this; }

  nat& operator-=(const nat& n){
    assert(*this >= n, "Subtraction results in negative");
    nat n2 = n;
    while(n2.data.size() < data.size())
      n2.data.pb(0);
    bool pull = false;
    for(int i = 0; i < n2.data.size(); ++i){
      llu b = 1;
      for(int j = 0; j < BITS_PER_BLOCK; ++j, b <<= 1){
        if(data[i] & b){
          if(n2.data[i] & b){
            if(pull) pass();
            else data[i] &= ~b;
          }else{
            if(pull) data[i] &= ~b, pull = false;
            else pass(); }
        }else{
          if(n2.data[i] & b){
            if(pull) pass();
            else data[i] |= b, pull = true;
          }else{
            if(pull) data[i] |= b;
            else pass(); } } } }
    assert(pull == false, "Subtraction pull bit set");
    validate();
    return *this; }

  // nat kar(const nat& x, const nat& y){
  //   if(x.data.size() > 1 && y.data.size() > 1){
  //     int N = min(x.data.size(), y.data.size()) >> 1;
  //     nat xl,xr,yl,yr;
  //     xr.data.clear(), xl.data.clear(), yl.data.clear(), yr.data.clear();
  //     for(int i = 0; i < N; ++i)
  //       xr.pb(x[i]);
  //     for(int i = N; i < x.data.size(); ++i)
  //       xl.pb(x[i]);
  //     for(int i = 0; i < N; ++i)
  //       yr.pb(y[i]);
  //     for(int i = N; i < y.data.size(); ++i)
  //       yl.pb(y[i]);
  //     return (kar(xl, yl) << (N << 7))
  //          + ((kar(xl + xr, yl + yr) - kar(xl, yl) - kar(xr, yr)) << (N << 6))
  //          + kar(xr, yr); } }

  // nat& operator*=(const nat& n){
  //   nat r = kar(*this, n);
  //   *this = r;
  //   validate();
  //   return *this; }

  nat& operator*=(const nat& n){
    nat r;
    int k = 0;
    for(int i = 0; i < data.size(); ++i){
      llu b = 1;
      for(int j = 0; j < BITS_PER_BLOCK; ++j, ++k, b <<= 1)
        if(data[i] & b) r += n << k; }
    *this = r;
    validate();
    return *this; }

  nat& operator/=(const nat& n){
    assert(n != 0, "Divide by zero");
    if(n > *this){
      *this = 0;
      return *this; }
    if(n > (*this >> 1)){
      *this = 1;
      return *this; }
    nat t, lo, hi, dist, guess, pguess, check, check1;
    t = n;
    lo = *this;
    while(t != 0){
      lo >>= 1;
      t >>= 1; }
    hi = lo << 1;
    dist = (hi - lo) >> 1;
    if(dist < 1) dist = 1;
    while(1){
      guess = dist + lo;
      if(guess == pguess) guess = hi - dist;
      pguess = guess;
      check = n * guess;
      check1 = n * (guess + 1);
      if(check <= *this && check1 > *this) break;
      if(check < *this) lo = guess;
      else hi = guess;
      if(dist > 1) dist >>= 1; }
    *this = guess;
    validate();
    return *this; }

  nat& operator%=(const nat& n){
    if(*this < n) return *this;
    nat t = *this / n;
    printf("\nthis = ");
    this->print_bits();
    printf("\n   t = ");
    t.print_bits();
    t *= n;
    printf("\n    -> ");
    t.print_bits();
    printf("\n");
    // n.print_bits();
    // printf("\n   t = ");
    // t.print_bits();
    // printf("\n");
    *this -= t;
    validate();
    return *this; }

  nat operator+(const nat& n) const {
    nat r = *this;
    r += n;
    return r; }
  nat operator-(const nat& n) const {
    nat r = *this;
    r -= n;
    return r; }
  nat operator*(const nat& n) const {
    nat r = *this;
    r *= n;
    return r; }
  nat operator/(const nat& n) const {
    nat r = *this;
    r /= n;
    return r; }
  nat operator%(const nat& n) const {
    nat r = *this;
    r %= n;
    return r; }
};

// struct num : thing {
//   bool neg;
//   nat top, bot;

//   num(): top(0), bot(1) {}
//   num(ll n): neg(n < 0), top(abs(n)), bot(1) {}

//   virtual void validate(){
//     top.validate(), bot.validate();
//     if(bot == 0) bot = 1;
//     nat g = top.gcd(bot);
//     if(g > 1) top /= g, bot /= g; }

//   num& operator=(const num& n){
//     top = n.top, bot = n.bot, neg = n.neg;
//     validate();
//     return *this; }

//   bool operator==(const num& n) const {
//     return top == n.top && bot = n.bot && neg = n.neg; }

// };

#endif

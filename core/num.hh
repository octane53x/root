// NUM

//! Division broken

#ifndef NUM_HH
#define NUM_HH

#include "list.hh"

struct Str;

// Natural number: integer >= 0
struct Nat : virtual List<Block> {

  // Constructors
  Nat();
  Nat(ull n);
  Nat(const Nat& n);

  // Operators

  // Ensure valid state
  virtual void _validate();
  // Convert to string
  // Defined in conv.hh
  virtual Str to_str(const PrintMode mode) const;

  void print() const {
    Nat t = *this;
    vec<int> digits;
    while(t != 0){
      Nat dn = t % 10;
      // printf("\n");
      // dn.print_bits();
      // printf("\n");
      int d = dn.to_int();
      digits.pb(d);
      t /= 10; }
    for(int i = digits.size()-1; i >= 0; --i)
      printf("%d", digits[i]); }

  Nat& operator=(const Nat& n){
    data.clear();
    data = n.data;
    validate();
    return *this; }

  bool operator==(const Nat& n) const {
    for(int i = 0; i < data.size(); ++i)
      if(data[i] != n.data[i]) return false;
    return true; }
  bool operator!=(const Nat& n) const {
    return !(*this == n); }

  bool operator<(const Nat& n) const {
    if(data.size() < n.data.size()) return true;
    if(data.size() > n.data.size()) return false;
    for(int i = data.size()-1; i >= 0; --i){
      if(data[i] < n.data[i]) return true;
      if(data[i] > n.data[i]) return false; }
    return false; }
  bool operator<=(const Nat& n) const {
    return (*this == n || *this < n); }
  bool operator>(const Nat& n) const {
    return !(*this <= n); }
  bool operator>=(const Nat& n) const {
    return !(*this < n); }

  Nat& operator&=(const Nat& n){
    if(n.data.size() < data.size())
      for(int i = 0; i < data.size()-n.data.size(); ++i)
        data.popb();
    for(int i = 0; i < data.size(); ++i)
      data[i] &= n.data[i];
    validate();
    return *this; }

  Nat& operator|=(const Nat& n){
    if(n.data.size() < data.size())
      for(int i = 0; i < data.size()-n.data.size(); ++i)
        data.popb();
    for(int i = 0; i < data.size(); ++i)
      data[i] |= n.data[i];
    validate();
    return *this; }

  Nat& operator^=(const Nat& n){
    if(n.data.size() < data.size())
      for(int i = 0; i < data.size()-n.data.size(); ++i)
        data.popb();
    for(int i = 0; i < data.size(); ++i)
      data[i] ^= n.data[i];
    validate();
    return *this; }

  Nat& operator>>=(const Nat& n){
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

  Nat& operator<<=(const Nat& n){
    assert(n < SHIFT_MAX, func, "Shift too large");
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

  Nat operator&(const Nat& n) const {
    Nat r = *this;
    r &= n;
    return r; }
  Nat operator|(const Nat& n) const {
    Nat r = *this;
    r |= n;
    return r; }
  Nat operator^(const Nat& n) const {
    Nat r = *this;
    r ^= n;
    return r; }
  Nat operator>>(const Nat& n) const {
    Nat r = *this;
    r >>= n;
    return r; }
  Nat operator<<(const Nat& n) const {
    Nat r = *this;
    r <<= n;
    return r; }

  Nat& operator+=(const Nat& n){
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

  Nat& operator-=(const Nat& n){
    assert(*this >= n, func, "Subtraction results in negative");
    Nat n2 = n;
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
    assert(pull == false, func, "Subtraction pull bit set");
    validate();
    return *this; }

  // Nat kar(const Nat& x, const Nat& y){
  //   if(x.data.size() > 1 && y.data.size() > 1){
  //     int N = min(x.data.size(), y.data.size()) >> 1;
  //     Nat xl,xr,yl,yr;
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

  // Nat& operator*=(const Nat& n){
  //   Nat r = kar(*this, n);
  //   *this = r;
  //   validate();
  //   return *this; }

  Nat& operator*=(const Nat& n){
    Nat r;
    int k = 0;
    for(int i = 0; i < data.size(); ++i){
      llu b = 1;
      for(int j = 0; j < BITS_PER_BLOCK; ++j, ++k, b <<= 1)
        if(data[i] & b) r += n << k; }
    *this = r;
    validate();
    return *this; }

  Nat& operator/=(const Nat& n){
    assert(n != 0, func, "Divide by zero");
    if(n > *this){
      *this = 0;
      return *this; }
    if(n > (*this >> 1)){
      *this = 1;
      return *this; }
    Nat t, lo, hi, dist, guess, pguess, check, check1;
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

  Nat& operator%=(const Nat& n){
    if(*this < n) return *this;
    Nat t = *this / n;
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

  Nat operator+(const Nat& n) const {
    Nat r = *this;
    r += n;
    return r; }
  Nat operator-(const Nat& n) const {
    Nat r = *this;
    r -= n;
    return r; }
  Nat operator*(const Nat& n) const {
    Nat r = *this;
    r *= n;
    return r; }
  Nat operator/(const Nat& n) const {
    Nat r = *this;
    r /= n;
    return r; }
  Nat operator%(const Nat& n) const {
    Nat r = *this;
    r %= n;
    return r; }
};

Nat::Nat(){
  push(0); }

Nat::Nat(const ull n){
  push(n); }

Nat::Nat(const Nat& n){
  *this = n; }

Nat::_validate(){
  while(data[data.size()-1] == 0)
    data.pop();
  if(data.empty()) data.push(0); }

Str Nat::to_bitstr() const {
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

// struct Num : thing {
//   bool neg;
//   Nat top, bot;

//   Num(): top(0), bot(1) {}
//   Num(ll n): neg(n < 0), top(abs(n)), bot(1) {}

//   virtual void validate(){
//     top.validate(), bot.validate();
//     if(bot == 0) bot = 1;
//     Nat g = top.gcd(bot);
//     if(g > 1) top /= g, bot /= g; }

//   Num& operator=(const Num& n){
//     top = n.top, bot = n.bot, neg = n.neg;
//     validate();
//     return *this; }

//   bool operator==(const Num& n) const {
//     return top == n.top && bot = n.bot && neg = n.neg; }

// };

#endif

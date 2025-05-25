// NUM

#ifndef NUM_HH
#define NUM_HH

#include "util.hh"

const int SHIFT_MAX = (1 << 30);

struct nat {
  vec<llu> data;

  nat(){ data.pb(0) }
  nat(int n){ data.pb(abs(n)); }
  nat(const nat& n){ *this = n; }

  void validate(){
    while(data[data.size()-1] == 0)
      data.pop_back();
    if(data.empty()) data.pb(0); }

  int to_int(){
    assert(*this <= (int)((1LL << 31) - 1), "Number too large to convert");
    return data[0]; }

  nat& operator=(const nat& n){
    data.clear();
    data = n.data;
    validate(); }

  bool operator==(const nat& n) const {
    for(int i = 0; i < data.size(); ++i)
      if(data[i] != n.data[i]) return false;
    return true; }

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
        data.pop_back();
    for(int i = 0; i < data.size(); ++i)
      data[i] &= n.data[i];
    validate();
    return *this; }

  nat& operator|=(const nat& n){
    if(n.data.size() < data.size())
      for(int i = 0; i < data.size()-n.data.size(); ++i)
        data.pop_back();
    for(int i = 0; i < data.size(); ++i)
      data[i] |= n.data[i];
    validate();
    return *this; }

  nat& operator^=(const nat& n){
    if(n.data.size() < data.size())
      for(int i = 0; i < data.size()-n.data.size(); ++i)
        data.pop_back();
    for(int i = 0; i < data.size(); ++i)
      data[i] ^= n.data[i];
    validate();
    return *this; }

  nat& operator>>=(const nat& n){
    if(n > SHIFT_MAX){
      *this = 0;
      return *this; }
    int sh = n.to_int();
    int S = sh / 64, s = sh % 64;
    for(int i = 0; i < data.size()-S; ++i)
      data[i] = data[i+S];
    for(int i = 0; i < S; ++i)
      data.pop_back();
    for(int i = 0; i < data.size()-1; ++i)
      data[i] = ((data[i] >> s) | (data[i+1] << (64-s)));
    data[data.size()-1] >>= s;
    validate();
    return *this; }

  nat& operator<<=(const nat& n){
    assert(n < SHIFT_MAX, "Shift too large");
    int sh = n.to_int();
    int S = sh / 64, s = sh % 64, k = data.size()-1;
    for(int i = 0; i < S; ++i)
      data.pb(0);
    for(int i = k; i >= 0; --i)
      data[i+S] = data[i];
    data.pb(0);
    for(int i = data.size()-1; i > 0; --i)
      data[i] = ((data[i] << s) | (data[i-1] >> (64-s)));
    data[0] <<= s;
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
    if(n.data.size() > data.size())
      for(int i = 0; i < n.data.size()-data.size(); ++i)
        data.pb(0);
    bool carry = false;
    for(int i = 0; i < data.size(); ++i){
      llu b = 1;
      for(int j = 0; j < 64; ++j){
        if(data[i] | b){
          if(n.data[i] | b){
            if(carry) pass();
            else data[i] &= (~b), carry = true;
          }else{
            if(carry) data[i] &= (~b);
            else pass(); }
        }else{
          if(n.data[i] | b){
            if(carry) pass();
            else data[i] |= b, carry = true;
          }else{
            if(carry) data[i] |= b, carry = false;
            else pass(); } }
        b <<= 1; } }
    if(carry)
      data.pb(1);
    validate();
    return *this; }

  nat kar(const nat& x, const nat& y){
    if(x.data.size() > 1 && y.data.size() > 1){
      int N = (min(x.data.size(), y.data.size()) >> 1);
      nat xl,xr,yl,yr;
      xr.data.clear(), xl.data.clear(), yl.data.clear(), yr.data.clear();
      for(int i = 0; i < N; ++i)
        xr.pb(x[i]);
      for(int i = N; i < x.data.size(); ++i)
        xl.pb(x[i]);
      for(int i = 0; i < N; ++i)
        yr.pb(y[i]);
      for(int i = N; i < y.data.size(); ++i)
        yl.pb(y[i]);
      return (kar(xl, yl) << (N << 7))
           + ((kar(xl + xr, yl + yr) - kar(xl, yl) - kar(xr, yr)) << (N << 6))
           + kar(xr, yr); } }

  nat& operator*=(const nat& n){
    nat r = kar(*this, n);
    *this = r;
    validate();
    return *this; } };

struct num {
  bool neg;
  nat top, bot;

  num(): top(0), bot(1) {}
  num(ll n): neg(n < 0), top(abs(n)), bot(1) {}

  void validate(){
    thing::validate();
    top.validate(), bot.validate();
    if(bot == 0) bot = 1;
    nat g = top.gcd(bot);
    if(g > 1) top /= g, bot /= g; }

  num& operator=(const num& n){
    top = n.top, bot = n.bot, neg = n.neg;
    validate();
    return *this; }

  bool operator==(const num& n) const {
    return top == n.top && bot = n.bot && neg = n.neg; } };

#endif

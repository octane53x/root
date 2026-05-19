// MTG UTIL

#ifndef MTG_UTIL
#define MTG_UTIL

#include <stdio.h>
#include <time.h>
#include <string.h>

#include <cfloat>
#include <fstream>

#include <string>
#include <vector>
#include <unordered_set>
#include <unordered_map>

#define CPS CLOCKS_PER_SEC
#define ull unsigned long long
#define str std::string
#define vec std::vector
#define uset std::unordered_set
#define umap std::unordered_map
#define pb push_back

const double INFD = DBL_MAX;

// Print to file or console
void print(const str& s){
#ifdef _WIN32
  ofstream fs(DEBUG_FILE.c_str(), ios::app);
  fs << s.data;
  fs.close();
#else
  printf("%s", s.c_str());
#endif
}

// Throw an error, with message
void err(const str& func, const str& msg){
  print(str("ERR: ") + func + str(": ") + msg + str("\n"));
  exit(-1); }

// Check a condition and throw an error if it fails
void assert(const bool b, const str& func, const str& msg){
  if(!b) err(func, msg); }

// Is character uppercase
bool is_upper(const char c){
  return (c >= 'A' && c <= 'Z'); }

// Is character lowercase
bool is_lower(const char c){
  return (c >= 'a' && c <= 'z'); }

// Is character a letter
bool is_alpha(const char c){
  return (is_lower(c) || is_upper(c)); }

// Is character a digit
bool is_digit(const char c){
  return (c >= '0' && c <= '9'); }

// Is string an integer
bool is_integer(const str& s){
  if(!(is_digit(s[0]) || s[0] == '-'))
    return false;
  for(int i = 1; i < s.size(); ++i)
    if(!is_digit(s[i]))
      return false;
  return true; }

// Convert to lowercase
str to_lower(const str& s){
  str r = s;
  for(int i = 0; i < r.size(); ++i)
    if(is_upper(r[i]))
      r[i] = r[i] - 'A' + 'a';
  return r; }

// Split string by delimiter
vec<str> split(const str& s, const str& delim){
  vec<str> r;
  vec<int> loc;
  for(int i = 0; i < s.size(); ++i){
    bool found = true;
    for(int j = i, k = 0; j < s.size() && k < delim.size(); ++j, ++k)
      if(s[j] != delim[k]){
        found = false;
        break; }
    if(found)
      loc.pb(i); }
  loc.pb(s.size());
  if(loc[0] > 0)
    r.pb(s.substr(0, loc[0]));
  for(int i = 1; i < loc.size(); ++i)
    r.pb(s.substr(loc[i - 1] + delim.size(),
        loc[i] - loc[i - 1] - delim.size()));
  return r; }

// Return whether an unordered set contains an element O(logN)
template <typename T>
bool contains(const uset<T>& u, const T& item){
  return u.find(item) != u.end(); }

// Return whether an unordered map contains an element O(logN)
template <typename K, typename V>
bool contains(const umap<K, V>& u, const K& item){
  return u.find(item) != u.end(); }

// Random number generator, but up to a 64 bit value (default RAND_MAX is less)
ull lrand(){
  int bits = 1;
  ull t = 1;
  while(t < RAND_MAX)
    ++bits, t <<= 1;
  ull r = 0;
  for(int i = 0; i < 64/bits; ++i)
    r = r * rand() + rand();
  return r; }

#endif

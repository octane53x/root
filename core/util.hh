// UTIL

#ifndef UTIL_HH
#define UTIL_HH

#include "def.hh"

const int
    // Infinity
    INF = INT_MAX;
const double
    // Infinity for double
    INFD = DBL_MAX,
    // Margin of error for comparing doubles
    SAFE_ZERO = 0.00000001,
    // Mathematical pi
    PI = 3.14159265358979323846;
const str
    // Debug log file location, print goes here when there's no console
    DEBUG_FILE = "../../debug.txt";

// Do nothing, used to follow if/else when the condition just needs to be seen
void pass(){}

// Print to either console or debug log file
void print(const str& s){
#ifdef _WIN32
  ofstream fs(DEBUG_FILE.c_str(), ios::app);
  fs << s;
  fs.close();
#else
  printf("%s", s.c_str());
#endif
}

// Print with a newline
void printl(const str& s){
  print(s + str("\n")); }

// Throw an error, with message
void err(const str& func, const str& msg){
  print(str("ERR: ") + func + str(": ") + msg + str("\n"));
  exit(-1); }

// Check a condition and throw an error if it fails
//! Takes a lot more time than it should, don't use in critical loops
inline void assert(const bool b, const str& func, const str& msg){
  if(!b) err(func, msg); }

// Print a given time
void print_time(const time_t time){
#ifdef _WIN32
  char buf[32];
  ctime_s(buf, sizeof(buf), &time);
  str s(buf);
#else
  str s(ctime(&time));
#endif
  printl(s); }

// Print the time
void print_time(){
  print_time(time(NULL)); }

// Print debug message
void debug(const str& msg){
  printl(msg);
  print_time(); }

// Initialize debug environment
void debug_init(){
  time_t t = time(NULL);
  remove(DEBUG_FILE.c_str());
  print(str("Executed at "));
  print_time(t); }

// Terminate the program
void quit(){
  time_t t = time(NULL);
  print(str("Terminated at "));
  print_time(t);
  exit(0); }

// Wait for a duration, in milliseconds
void sleep(const int ms){
  clock_t start = clock();
  while(1){
    clock_t t = clock();
    int p = (int)floor((double)(t - start) / CLOCKS_PER_SEC * 1000.0);
    if(p >= ms) break; } }

// Return whether a vector contains an element O(N)
template <typename T>
bool contains(const vec<T>& v, const T& item){
  for(int i = 0; i < v.size(); ++i)
    if(v[i] == item) return true;
  return false; }

// Return whether an unordered set contains an element
template <typename T>
bool contains(const uset<T>& u, const T& item){
  return u.find(item) != u.end(); }

// Return whether an unordered map contains an element
template <typename K, typename V>
bool contains(const umap<K, V>& u, const K& item){
  return u.find(item) != u.end(); }

// Double equals comparator
bool deq(const double a, const double b){
  return fabs(a - b) < SAFE_ZERO; }

// Double less than or equals comparator
bool dleq(const double a, const double b){
  return a < b || deq(a, b); }

// Double greater than or equals comparator
bool dgeq(const double a, const double b){
  return a > b || deq(a, b); }

// Double less than comparator
bool dlt(const double a, const double b){
  return a < b && !deq(a, b); }

// Double greater than comparator
bool dgt(const double a, const double b){
  return a > b && !deq(a, b); }

// Greatest common denominator
int gcd(const int a, const int b){
  return b ? gcd(b, a % b) : a; }

// Random number generator, but up to a 64 bit value (default RAND_MAX is less)
llu lrand(){
  int bits = 1;
  llu t = 1;
  while(t < RAND_MAX)
    ++bits, t <<= 1;
  llu r = 0;
  for(int i = 0; i < 64/bits; ++i)
    r = r * rand() + rand();
  return r; }

// Curved random number generator, favoring smaller numbers
//! Curve param
int crand(){
  const int RMAX = 1000000;
  int r = 1;
  while(r == 1)
    r = (int)floor((double)RMAX / (rand() % RMAX + 1));
  return r - 1; }

#endif

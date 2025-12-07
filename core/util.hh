// UTIL

#ifndef UTIL_HH
#define UTIL_HH

#include "def.hh"

const int
    INF = INT_MAX;
const double
    INFD = DBL_MAX,
    SAFE_ZERO = 0.00000001,
    PI = 3.14159265358979323846;
const str
    DEBUG_FILE = "../../debug.txt";

void pass(){}

void print(str s){
#ifdef _WIN32
  ofstream fs(DEBUG_FILE.c_str(), ios::app);
  fs << s;
  fs.close();
#else
  printf("%s", s.c_str());
#endif
}

void err(const char* msg){
  print(str("ERR: ") + str(msg) + str("\n"));
  exit(-1); }

void assert(bool b, const char* msg){
  if(!b) err((str("ASSERT: ") + str(msg)).c_str()); }

void sleep(int ms){
  clock_t start = clock();
  while(1){
    clock_t t = clock();
    int p = (int)floor((double)(t - start) / CLOCKS_PER_SEC * 1000.0);
    if(p >= ms) break; } }

template <typename T>
bool contains(vec<T> v, T item){
  for(int i = 0; i < v.size(); ++i)
    if(v[i] == item) return true;
  return false; }

bool deq(double a, double b){
  return fabs(a - b) < SAFE_ZERO; }
bool dleq(double a, double b){
  return a < b || deq(a, b); }
bool dgeq(double a, double b){
  return a > b || deq(a, b); }
bool dlt(double a, double b){
  return a < b && !deq(a, b); }
bool dgt(double a, double b){
  return a > b && !deq(a, b); }

int gcd(int a, int b){
  return b ? gcd(b, a % b) : a; }

llu lrand(){
  int bits = 1;
  llu t = 1;
  while(t < RAND_MAX)
    ++bits, t <<= 1;
  llu r = 0;
  for(int i = 0; i < 64/bits; ++i)
    r = r * rand() + rand();
  return r; }

//! Param
int crand(){
  const int RMAX = 1000000;
  int r = 1;
  while(r == 1)
    r = (int)floor((double)RMAX / (rand() % RMAX + 1));
  return r - 1; }

#endif

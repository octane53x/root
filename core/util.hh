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

void pass(){}

void err(const char* msg){
  printf("ERR: %s\n", msg);
  exit(-1); }

void assert(bool b, const char* msg){
  if(!b) err((str("ASSERT: ") + str(msg)).c_str()); }

void sleep(int ms){
  clock_t start = clock();
  while(1){
    clock_t t = clock();
    int p = (int)floor((double)(t - start) / CLOCKS_PER_SEC * 1000.0);
    if(p >= ms) break; } }

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
  return (llu)rand() * rand() + rand(); }

const int RMAX = 1000000; //?
int crand(){
  int r = 1;
  while(r == 1)
    r = (int)floor((double)RMAX / (rand() % RMAX + 1));
  return r - 1; }

#endif

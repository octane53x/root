// UTIL

#ifndef UTIL_HH
#define UTIL_HH

#include "def.hh"

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

const int RMAX = 1000000;
int crand(){
  int r = 1;
  while(r == 1)
    r = (int)floor((double)RMAX / (rand() % RMAX + 1));
  return r - 1; }

#endif

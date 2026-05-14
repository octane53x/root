// TEMP INCL

#ifndef TEMP_INCL_HH
#define TEMP_INCL_HH

#include <time.h>
#include <math.h>
#include <string.h>

#include <fstream>

#include <string>
#include <vector>
#include <unordered_set>
#include <unordered_map>

#include <windows.h>

#define uchar unsigned char
#define ui unsigned int
#define str string
#define vec vector
#define uset unordered_set
#define umap unordered_map

#define pb push_back

using namespace std;

const int CPS = CLOCKS_PER_SEC;

const double SAFE_ZERO = 0.00000001;

const str DEBUG_FILE = "c:/home/root/debug.txt";

void print(const str& s){
  ofstream fs(DEBUG_FILE.c_str(), ios::app);
  fs << s;
  fs.close(); }

void printl(const str& s){
  print(s + str("\n")); }

void print_time(const time_t time){
  char buf[32];
  ctime_s(buf, sizeof(buf), &time);
  str s(buf);
  printl(s); }

void err(const str& func, const str& msg){
  print(str("ERR: ") + func + str(": ") + msg + str("\n"));
  exit(-1); }

void assert(const bool b, const str& func, const str& msg){
  if(!b) err(func, msg); }

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

// Return whether a vector contains an element O(N)
template <typename T>
bool contains(const vec<T>& v, const T& item){
  for(int i = 0; i < v.size(); ++i)
    if(v[i] == item) return true;
  return false; }

// Return whether an unordered set contains an element O(logN)
template <typename T>
bool contains(const uset<T>& u, const T& item){
  return u.find(item) != u.end(); }

// Return whether an unordered map contains an element O(logN)
template <typename K, typename V>
bool contains(const umap<K, V>& u, const K& item){
  return u.find(item) != u.end(); }

#endif

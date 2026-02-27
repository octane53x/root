// UTILITY

#ifndef UTIL_HH
#define UTIL_HH

#include "double.hh"
#include "math.hh"
#include "obj.hh"

// Do nothing, used to follow if/else when the condition just needs to be seen
void pass(){}

// Print to either console or debug log file
void print(const str& s){
#ifdef _WIN32
  ofstream fs(DEBUG_FILE.c_str(), ios::app);
  fs << s.data;
  fs.close();
#else
  printf("%s", s.c_str());
#endif
}

// Print int
void print(const int n){
  print(to_string(n)); }

// Print double
void print(const double d){
  print(to_string(d)); }

// Print with a newline
void printl(const str& s){
  print(s + str("\n")); }

// Print int
void printl(const int n){
  printl(to_string(n)); }

// Print double
void printl(const double d){
  printl(to_string(d)); }

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

// Print debug int
void debug(const int n){
  debug(to_string(n)); }

// Print debug double
void debug(const double d){
  debug(to_string(d)); }

// Print debug ints
void debug(const initializer_list<int>& ints){
  str msg = "";
  for(int i : ints)
    msg += to_string(i) + " ";
  debug(msg); }

// Initialize debug environment
void debug_init(){
  time_t t = time(NULL);
  remove(DEBUG_FILE.c_str());
  print(str("Executed at "));
  print_time(t); }

// Throw an error, with message
void err(const str& func, const str& msg){
  print(str("ERR: ") + func + str(": ") + msg + str("\n"));
  exit(-1); }

// Check a condition and throw an error if it fails
void assert(const bool b, const str& func, const str& msg){
  if(!b) err(func, msg); }

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

#endif

// CRAB UTIL

#ifndef CRAB_UTIL
#define CRAB_UTIL

#include "../../core/util.hh"

vec<str> file_to_text(const str& file){
  ifstream fs(file);
  vec<str> r;
  str line;
  while(getline(fs, line))
    r.pb(line);
  return r; }

bool starts_with(const str& s, const str& t){
  if(t.size() > s.size()) return false;
  for(int i = 0; i < t.size(); ++i)
    if(s[i] != t[i]) return false;
  return true; }

bool is_alpha(const char c){
  return (c >= 'A' && c <= 'Z') || (c >= 'a' && c <= 'z'); }

bool is_digit(const char c){
  return c >= '0' && c <= '9'; }

str next_tok(str line){
  if(line == "")
    return "";
  int i = 1;
  if(is_alpha(line[0]) || line[0] == '_')
    while(i < line.size()
        && (is_alpha(line[i]) || is_digit(line[i]) || line[i] == '_'))
      ++i;
  else if(is_digit(line[0]))
    while(i < line.size() && is_digit(line[i]))
      ++i;
  else
    while(i < line.size() && !is_alpha(line[i]) && !is_digit(line[i])
        && line[i] != '_' && line[i] != ' ')
      ++i;
  return line.substr(0, i); }

#endif

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

bool is_upper(const char c){
  return c >= 'A' && c <= 'Z'; }

bool is_lower(const char c){
  return c >= 'a' && c <= 'z'; }

bool is_alpha(const char c){
  return is_upper(c) || is_lower(c); }

bool is_digit(const char c){
  return c >= '0' && c <= '9'; }

bool is_type(const str& s){
  if(s == "") return false;
  int i = 0;
  while(i < s.size() && s[i] == '_')
    ++i;
  if(i >= s.size() || !is_upper(s[i])) return false;
  while(++i < s.size())
    if(!(is_alpha(s[i]) || is_digit(s[i]) || s[i] == '_')) return false;
  return true; }

bool is_name(const str& s){
  if(s == "") return false;
  int i = 0;
  while(i < s.size() && s[i] == '_')
    ++i;
  if(i >= s.size() || !is_lower(s[i])) return false;
  while(++i < s.size())
    if(!(is_alpha(s[i]) || is_digit(s[i]) || s[i] == '_')) return false;
  return true; }

str strip(str s){
  while(s != "" && (s[0] == ' ' || s[0] == '\n' || s[0] == '\r'))
    s = s.substr(1);
  while(s != "" && (s.back() == ' ' || s.back() == '\n' || s.back() == '\r'))
    s = s.substr(0, s.size() - 1);
  return s; }

str next_tok(str line){
  line = strip(line);
  if(line == "")
    return "";
  int i = 1;
  // Type or name
  if(is_alpha(line[0]) || line[0] == '_')
    while(i < line.size()
        && (is_alpha(line[i]) || is_digit(line[i]) || line[i] == '_'))
      ++i;
  // Numeric constant
  else if(is_digit(line[0]))
    while(i < line.size() && is_digit(line[i]))
      ++i;
  return line.substr(0, i); }

vec<str> split_tok(str line){
  line = strip(line);
  vec<str> r;
  while(line != ""){
    str tok = next_tok(line);
    line = strip(line.substr(tok.size()));
    r.pb(tok); }
  return r; }

#endif

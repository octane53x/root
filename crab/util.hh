// UTIL

#ifndef CRAB_UTIL_HH
#define CRAB_UTIL_HH

#include "../core/util.hh"

str strip(const str& s){
  int i = 0;
  while(i < s.size() && s[i] == ' ')
    ++i;
  int j = s.size() - 1;
  while(j >= 0 && s[j] == ' ')
    --j;
  if(i <= j)
    return s.substr(i, j - i + 1);
  return ""; }

vec<str> split(str s, const str& delim){
  vec<str> r;
  int i;
  while((i = s.find(delim)) != str::npos){
    r.pb(s.substr(0, i));
    s = s.substr(i + delim.size()); }
  if(s != "")
    r.pb(s);
  return r; }

bool starts_with(const str& s, const str& t){
  if(t.size() > s.size()) return false;
  for(int i = 0; i < t.size(); ++i)
    if(s[i] != t[i]) return false;
  return true; }

bool ends_with(const str& s, const str& t){
  if(t.size() > s.size()) return false;
  for(int i = t.size() - 1; i >= 0; --i)
    if(s[i] != t[i]) return false;
  return true; }

bool is_upper(const char c){
  return (c >= 'A' && c <= 'Z'); }

bool is_lower(const char c){
  return (c >= 'a' && c <= 'z'); }

bool is_alpha(const char c){
  return is_upper(c) || is_lower(c); }

bool is_digit(const char c){
  return (c >= '0' && c <= '9'); }

bool is_type(const str& s){
  int i = 0;
  while(i < s.size() && s[i] == '_')
    ++i;
  if(i == s.size() || !is_upper(s[i])) return false;
  ++i;
  while(i < s.size() && (is_alpha(s[i]) || is_digit(s[i]) || s[i] == '_'))
    ++i;
  return i == s.size(); }

bool is_mutable_name(const str& s){
  int i = 0;
  while(i < s.size() && s[i] == '_')
    ++i;
  if(i == s.size() || !is_lower(s[i])) return false;
  ++i;
  while(i < s.size() && (is_lower(s[i]) || is_digit(s[i]) || s[i] == '_'))
    ++i;
  return i == s.size(); }

bool is_const_name(const str& s){
  int i = 0;
  while(i < s.size() && s[i] == '_')
    ++i;
  if(i == s.size() || !is_upper(s[i])) return false;
  ++i;
  while(i < s.size() && (is_upper(s[i]) || is_digit(s[i]) || s[i] == '_'))
    ++i;
  return i == s.size(); }

// Token definition:
// - Alphanumeric string (including underscore)
// - One parenthesis, square bracket, or curly bracket
// - Any number of other consecutive characters but space
// Angle brackets are considered operators and must be handled with care
str next_tok(str s){
  s = strip(s);
  if(s == "") return "";
  int i = 0;
  if(is_alpha(s[0]) || is_digit(s[0]) || s[0] == '_')
    while(i < s.size() && (is_alpha(s[i]) || is_digit(s[i]) || s[i] == '_'))
      ++i;
  else if(s[0] == '(' || s[0] == ')' || s[0] == '[' || s[0] == ']'
      || s[0] == '{' || s[0] == '}')
    ++i;
  else
    while(!is_alpha(s[i]) && !is_digit(s[i]) && s[i] != '_'
        && s[i] != '(' && s[i] != ')' && s[i] != '[' && s[i] != ']'
        && s[i] != '{' && s[i] != '}')
      ++i;
  return s.substr(0, i); }

str delete_tok(str s){
  str tok = next_tok(s);
  s = strip(s);
  return s.substr(tok.size()); }

#endif

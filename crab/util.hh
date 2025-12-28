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

bool starts_with(const str& s, const str& t){
  if(t.size() > s.size()) return false;
  for(int i = 0; i < t.size(); ++i)
    if(s[i] != t[i]) return false;
  return true; }

bool is_upper(char c){
  return (c >= 'A' && c <= 'Z'); }

bool is_lower(char c){
  return (c >= 'a' && c <= 'z'); }

bool is_alpha(char c){
  return is_upper(c) || is_lower(c); }

bool is_digit(char c){
  return (c >= '0' && c <= '9'); }

bool is_name(char c){
  return is_alpha(s[0]) || is_digit(s[0]) || s[0] == '_'; }

// Token definition:
// - Alphanumeric string (including underscore)
// - One parenthesis, square bracket, or curly bracket
// - Any number of other consecutive characters but space
// Angle brackets are considered operators and must be handled with care
str next_tok(str s){
  s = s.strip();
  if(s == "") return "";
  int i = 0;
  if(is_name(s[0])
    while(i < s.size() && is_name(s[i]))
      ++i;
  else if(s[0] == '(' || s[0] == ')' || s[0] == '[' || s[0] == ']'
      || s[0] == '{' || s[0] == '}'
    ++i;
  else
    while(!is_name(s[i]) && s[i] != '(' && s[i] != ')'
        && s[i] != '[' && s[i] != ']' && s[i] != '{' && s[i] != '}')
      ++i;
  return s.substr(0, i); }

str delete_tok(str s){
  str tok = next_tok(s);
  s = s.strip();
  return s.substr(tok.size()); }

#endif

// STUDIO UTILITY

#ifndef STUDIO_UTIL_HH
#define STUDIO_UTIL_HH

#include "const.hh"

str clean_path(const str& s){
  str r = s;
  for(int i = 0; i < s.size(); ++i)
    if(r[i] == '\\')
      r[i] = '/';
  if(r.back() != '/')
    r += "/";
  return r; }

bool in_selection(const ipoint& p0, const ipoint& pf, const ipoint& p){
  bool after_p0 = (p.y > p0.y || (p.y == p0.y && p.x >= p0.x));
  bool before_pf = (p.y < pf.y || (p.y == pf.y && p.x <= pf.x));
  return (after_p0 && before_pf); }

bool name_or_val(const char c){
  return is_alpha(c) || is_digit(c) || c == '_'; }

bool type_or_name(const str& s){
  if(!(is_alpha(s[0]) || s[0] == '_')) return false;
  for(int i = 1; i < s.size(); ++i)
    if(!name_or_val(s[i])) return false;
  return true; }

bool all_spaces(const str& s){
  int spaces = 0;
  for(; spaces < s.size() && s[spaces] == ' '; ++spaces);
  return spaces == s.size(); }

str next_tok(const str& s){
  if(s == "") return "";
  uset<str> ops =
      {"//", "/*", "*/", "::", "->", "==", "!=", ">=", "<=", "&&", "||",
      "<<", ">>", "<<=", ">>=", "+=", "-=", "*=", "/=", "%=", "&=", "|=", "^="};
  int i = 0;
  // Numeric constant
  if(is_digit(s[0])){
    while(i < s.size() && is_digit(s[i]))
      ++i;
  // Type or name
  }else if(is_alpha(s[0]) || s[0] == '_'){
    ++i;
    while(i < s.size() && name_or_val(s[i]))
      ++i;
  // Spaces
  }else if(s[0] == ' '){
    while(i < s.size() && s[i] == ' ')
      ++i;
  // Three char operator
  }else if(s.size() > 2 && ops.find(s.substr(0, 3)) != ops.end())
    i += 3;
  // Two char operator
  else if(s.size() > 1 && ops.find(s.substr(0, 2)) != ops.end())
    i += 2;
  // Otherwise return single character
  else
    ++i;
  return s.substr(0, i); }

str delete_tok(const str& s){
  str tok = next_tok(s);
  return s.substr(tok.size()); }

RECT get_rect(const ipoint& pos, const ipoint& size){
  RECT r;
  r.left = pos.x;
  r.top = pos.y;
  r.right = pos.x + size.x;
  r.bottom = pos.y + size.y;
  return r; }

#endif

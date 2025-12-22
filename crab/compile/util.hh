// COMPILER UTIL

#ifndef COMPILER_UTIL
#define COMPILER_UTIL

#include "compiler.hh"

// Is character uppercase
bool Compiler::is_upper(const char c) const {
  return (c >= 'A' && c <= 'Z'); }

// Is character lowercase
bool Compiler::is_lower(const char c) const {
  return (c >= 'a' && c <= 'z'); }

// Is character a digit
bool Compiler::is_digit(const char c) const {
  return (c >= '0' && c <= '9'); }

// Get first token from string
// Returns "" if no token
str Compiler::next_tok(const str& line) const {
  if(line == "") return "";
  int i = 0;
  while(i < line.size() && line[i] == ' ')
    ++i;
  if(i == line.size()) return "";
  int j = i + 1;
  while(j < line.size() && line[j] != ' ')
    ++j;
  return line.substr(i, j - i); }

// Delete first token from string
str Compiler::delete_tok(const str& line) const {
  if(line == "") return "";
  int i = 0;
  while(i < line.size() && line[i] == ' ')
    ++i;
  while(i < line.size() && line[i] != ' ')
    ++i;
  if(i == line.size()) return "";
  assert(i >= 2, "Compiler.delete_tok", "malfunction");
  return line.substr(i); }

// Get first value from string
// Returns "" if no value
str Compiler::next_val(const str& line) const {
  if(line == "") return "";
  int i = 0;
  while(i < line.size() && line[i] == ' ')
    ++i;
  vec<char> b;
  for(int j = i; j < line.size(); ++j){
    if(line[j] == '(' || line[j] == '[' || line[j] == '{'){
      b.pb(line[j]);
      continue; }
    if(line[j] == ')' || line[j] == ']' || line[j] == '}'){
      if(b.empty()) return line.substr(i, j - i);
      char c = (line[j] == ')') ? '(' : ((line[j] == ']') ? '[' : '{');
      if(b.back() == c){
        b.popb();
        continue; }
      return ""; }
    if(line[j] == ',' && b.empty())
      return line.substr(i, j - i); }
  return b.empty() ? line.substr(i) : ""; }

// Delete first value from string
str Compiler::delete_val(const str& line) const {
  if(line == "") return "";
  str val = next_val(line);
  if(val == "") return line;
  int i = 0;
  while(i < line.size() && line[i] == ' ')
    ++i;
  return line.substr(i + val.size()); }

#endif

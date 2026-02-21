// STRING

#ifndef STR_HH
#define STR_HH

#include "char.hh"

// Wrapper for std::string
struct str {

  // Internal std::string
  string data;

  // Constructors
  str(){}
  str(char c);
  str(const char* s);
  str(const string& s);
  str(const str& s);
  str(int n);
  str(llu n);
  str(double d);

  // Const operators
  char operator[](size_t index) const;
  str operator+(const str& s) const;

  // Modifying operators
  char& operator[](size_t index);
  str& operator+=(const str& s);

  // Wrappers
  const char* c_str() const;
  char at(size_t index) const;
  size_t size() const;
  str substr(size_t index) const;
  str substr(size_t index, size_t len) const;

  // Evaluators
  bool is_integer() const;
  bool is_decimal() const;
  bool starts_with(const str& s) const;
  bool ends_with(const str& s) const;

  // Converters
  str to_upper() const;
  str to_lower() const;
  vec<str> split(const str& delim) const; };

// Construct from character
str::str(char c){
  data = string(1, c); }

// Construct from character array
str::str(const char* s){
  data = string(s); }

// Construct from std::string
str::str(const string& s){
  data = s; }

// Construct from other string
str::str(const str& s){
  data = s.data; }

// Construct from int
str::str(int n){
  data = to_string(n); }

// Construct from 64-bit unsigned
str::str(llu n){
  data = to_string(n); }

// Construct from double
str::str(double d){
  data = to_string(d); }

// Return character at index
char str::operator[](size_t index) const {
  return data[index]; }

// Concatenate strings
str str::operator+(const str& s) const {
  str r;
  r.data = this->data + s.data;
  return r; }

// Return character reference at index
char& str::operator[](size_t index){
  return data[index]; }

// Concatenate strings
str& str::operator+=(const str& s){
  data += s.data;
  return *this; }

// Return the character array within std::string
const char* str::c_str() const {
  return data.c_str(); }

// Return character at index
char str::at(size_t index) const {
  return data[index]; }

// Return length of string
size_t str::size() const {
  return data.size(); }

// Return substring from index to end
str str::substr(size_t index) const {
  return data.substr(index); }

// Return substring at index with length
str str::substr(size_t index, size_t len) const {
  return data.substr(index, len); }

// Is string an integer
bool str::is_integer() const {
  if(!(is_digit(at(0)) || at(0) == '-'))
    return false;
  for(int i = 1; i < size(); ++i)
    if(!is_digit(at(i)))
      return false;
  return true; }

// Is string a decimal
bool str::is_decimal() const {
  if(!(is_digit(at(0)) || at(0) == '-' || at(0) == '.'))
    return false;
  bool dec_found = false;
  if(at(0) == '.')
    dec_found = true;
  for(int i = 1; i < size(); ++i){
    if(at(i) == '.'){
      if(dec_found)
        return false;
      dec_found = true; }
    else if(!is_digit(at(i)))
      return false; }
  return true; }

// Does string start with string
bool str::starts_with(const str& s) const {
  if(s.size() > size()) return false;
  for(int i = 0; i < s.size(); ++i)
    if(at(i) != s[i]) return false;
  return true; }

// Does string end with string
bool str::ends_with(const str& s) const {
  if(s.size() > size()) return false;
  for(int i = s.size() - 1, j = size() - 1; i >= 0; --i, --j)
    if(at(j) != s[i]) return false;
  return true; }

// Convert to uppercase
str str::to_upper() const {
  str r = *this;
  for(int i = 0; i < r.size(); ++i)
    if(is_lower(r[i]))
      r[i] = r[i] - 'a' + 'A';
  return r; }

// Convert to lowercase
str str::to_lower() const {
  str r = *this;
  for(int i = 0; i < r.size(); ++i)
    if(is_upper(r[i]))
      r[i] = r[i] - 'A' + 'a';
  return r; }

// Split string by delimiter
vec<str> str::split(const str& delim) const {
  vec<str> r;
  vec<int> loc;
  for(int i = 0; i < size(); ++i){
    bool found = true;
    for(int j = i, k = 0; j < size() && k < delim.size(); ++j, ++k)
      if(at(j) != delim[k]){
        found = false;
        break; }
    if(found)
      loc.pb(i); }
  loc.pb(size());
  if(loc[0] > 0)
    r.pb(substr(0, loc[0]));
  for(int i = 1; i < loc.size(); ++i)
    r.pb(substr(loc[i - 1] + delim.size(), loc[i] - loc[i - 1] - delim.size()));
  return r; }

#endif

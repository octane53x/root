// STRING

#ifndef STR_HH
#define STR_HH

#include "char.hh"
#include "list.hh"
#include "num.hh"

// Wrapper for std::string
struct str : virtual thing {

  //! Internal std::string, repl list<char>
  std::string data;

  // Constructors
  str(){}
  str(const char c);
  str(const char* s);
  str(const std::string& s);
  str(const str& s);
  str(const num& n);

  // Const operators
  bool operator==(const str& s) const;
  bool operator!=(const str& s) const;
  str operator+(const str& s) const;

  // Modifying operators
  char& operator[](const num& i);
  str& operator+=(const str& s);

  // Wrappers
  const char* c_str() const;
  char at(const num& i) const;
  num size() const;
  num find(const str& s) const;
  str substr(const num& i) const;
  str substr(const num& i, const num& len) const;

  // Evaluators
  bool is_integer() const;
  bool is_decimal() const;
  bool starts_with(const str& s) const;
  bool ends_with(const str& s) const;

  // Converters
  str to_upper() const;
  str to_lower() const;
  list<str> split(const str& delim) const; };

// Construct from character
str::str(char c){
  data = string(1, c); }

// Construct from character array
str::str(const char* s){
  data = string(s); }

// Construct from std::string
str::str(const std::string& s){
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

// Return whether strings are equal
bool str::operator==(const str& s) const {
  return data == s.data; }

// Return whether strings are not equal
bool str::operator!=(const str& s) const {
  return data != s.data; }

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

// Return first position of substring, else string::npos
size_t str::find(const str& s) const {
  return data.find(s.data); }

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
list<str> str::split(const str& delim) const {
  list<str> r;
  list<int> loc;
  for(int i = 0; i < size(); ++i){
    bool found = true;
    for(int j = i, k = 0; j < size() && k < delim.size(); ++j, ++k)
      if(at(j) != delim[k]){
        found = false;
        break; }
    if(found)
      loc.add(i); }
  loc.add(size());
  if(loc[0] > 0)
    r.add(substr(0, loc[0]));
  for(int i = 1; i < loc.size(); ++i)
    r.add(substr(loc[i - 1] + delim.size(),
        loc[i] - loc[i - 1] - delim.size()));
  return r; }

template <typename T>
str list<T>::to_str() const {
  str r = "{";
  for(int i = 0; i < size() - 1; ++i)
    r += data[i].to_str() + ", ";
  return r + data[size() - 1].to_str() + "}"; }

// Allow str to be hashed as a map key
namespace std {
  template <>
  struct hash<str> {
    size_t operator()(const str& s) const {
      return hash<std::string>()(s.data); } }; }

#endif

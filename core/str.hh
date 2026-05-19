// STRING

#ifndef STR_HH
#define STR_HH

#include "char.hh"
#include "list.hh"
//#include "num.hh"
#define Num int //!

// Wrapper for std::string
struct Str : virtual Thing {

  // Storage
  List<Char> data;

  // Constructors
  Str(){}
  Str(const Char c);
  Str(const Str& s);
  // Defined in ops/prim.hh
  Str(const char* s);

  // Const operators
  Char operator[](const Num& i) const;
  Flag operator==(const Str& s) const;
  Flag operator!=(const Str& s) const;
  Str operator+(const Str& s) const;

  // Modifying operators
  char& operator[](const Num& i);
  Str& operator=(const Str& s);
  Str& operator+=(const Str& s);

  // Wrappers
  const char* c_str() const;
  Num size() const;
  Num find(const Str& s) const;
  Str substr(const Num& i) const;
  Str substr(const Num& i, const Num& len) const;

  // Evaluators
  bool is_integer() const;
  bool is_decimal() const;
  bool starts_with(const Str& s) const;
  bool ends_with(const Str& s) const;

  // Converters
  Str to_upper() const;
  Str to_lower() const;
  List<Str> split(const Str& delim) const; };

// Construct from character
Str::Str(const Char c){
  data.push(c); }

// Construct from other string
Str::Str(const Str& s){
  data = s.data; }

// Return character at index
Char Str::operator[](const Num& index) const {
  return data[index]; }

// Return whether strings are equal
bool Str::operator==(const Str& s) const {
  return data == s.data; }

// Return whether strings are not equal
bool Str::operator!=(const Str& s) const {
  return !(*this == s); }

// Concatenate Strings
Str Str::operator+(const Str& s) const {
  Str r;
  r.data = this->data + s.data;
  return r; }

// Return reference to character at index
char& Str::operator[](const Num& index){
  return data[index]; }

// Concatenate Strings
Str& Str::operator+=(const Str& s){
  data += s.data;
  return *this; }

// Return the character array within std::string
const char* Str::c_str() const {
  return data.c_str(); }

// Return character at index
char Str::at(size_t index) const {
  return data[index]; }

// Return length of String
size_t Str::size() const {
  return data.size(); }

// Return first position of substring, else String::npos
size_t Str::find(const Str& s) const {
  return data.find(s.data); }

// Return substring from index to end
Str Str::substr(size_t index) const {
  return data.substr(index); }

// Return substring at index with length
Str Str::substr(size_t index, size_t len) const {
  return data.substr(index, len); }

// Is String an integer
bool Str::is_integer() const {
  if(!(is_digit(at(0)) || at(0) == '-'))
    return false;
  for(int i = 1; i < size(); ++i)
    if(!is_digit(at(i)))
      return false;
  return true; }

// Is String a decimal
bool Str::is_decimal() const {
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

// Does String start with String
bool Str::starts_with(const Str& s) const {
  if(s.size() > size()) return false;
  for(int i = 0; i < s.size(); ++i)
    if(at(i) != s[i]) return false;
  return true; }

// Does String end with String
bool Str::ends_with(const Str& s) const {
  if(s.size() > size()) return false;
  for(int i = s.size() - 1, j = size() - 1; i >= 0; --i, --j)
    if(at(j) != s[i]) return false;
  return true; }

// Convert to uppercase
Str Str::to_upper() const {
  Str r = *this;
  for(int i = 0; i < r.size(); ++i)
    if(is_lower(r[i]))
      r[i] = r[i] - 'a' + 'A';
  return r; }

// Convert to lowercase
Str Str::to_lower() const {
  Str r = *this;
  for(int i = 0; i < r.size(); ++i)
    if(is_upper(r[i]))
      r[i] = r[i] - 'A' + 'a';
  return r; }

// Split String by delimiter
List<Str> Str::split(const Str& delim) const {
  List<Str> r;
  List<int> loc;
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
Str List<T>::to_str() const {
  Str r = "{";
  for(int i = 0; i < size() - 1; ++i)
    r += data[i].to_str() + ", ";
  return r + data[size() - 1].to_str() + "}"; }

// Allow Str to be hashed as a map key
namespace std {
  template <>
  struct hash<Str> {
    size_t operator()(const Str& s) const {
      return hash<std::string>()(s.data); } }; }

#endif

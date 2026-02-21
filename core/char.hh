// CHARACTER UTILITY

#ifndef CHAR_HH
#define CHAR_HH

#include "def.hh"

// Is character uppercase
bool is_upper(const char c){
  return (c >= 'A' && c <= 'Z'); }

// Is character lowercase
bool is_lower(const char c){
  return (c >= 'a' && c <= 'z'); }

// Is character a letter
bool is_alpha(const char c){
  return (is_lower(c) || is_upper(c)); }

// Is character a digit
bool is_digit(const char c){
  return (c >= '0' && c <= '9'); }

#endif

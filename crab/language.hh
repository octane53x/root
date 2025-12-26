// LANGUAGE DEPENDENCIES

#ifndef LANGUAGE_HH
#define LANGUAGE_HH

#include "var.hh"

typedef str Control;

// C++ Dependencies
const vec<Control> CONTROL = {
  "USER",
  "IF", "ELSE", "FOR", "WHILE", "BREAK", "CONTINUE", "RETURN",
  "TYPENAME", "EXIT", "PRINT",
  // Block operations
  "AND", "OR", "NOT", "EQUAL", "LESS", "GREATER",
  "ADD", "SUBTRACT", "MULTIPLY", "DIVIDE", "MODULO",
  "BITAND", "BITOR", "BITXOR", "BITINV", "LSHIFT", "RSHIFT" };

struct Language {

  Var process_fn(const Control& fn, const vec<Var>& params);
  bool fn_if(const vec<Var>& params);
  void fn_else(const vec<Var>& params); };

Var Language::process_fn(const Control& fn, const vec<Var>& params){
  if(fn == "USER")
    err("Language.process_fn", "called with USER code");
  if(fn == "IF")
    return Var(fn_if(params));
  if(fn == "ELSE"){
    fn_else(params);
    return Var(); }
  //! more
  err("Language.process_fn", "control fn not handled");
  // Unreachable
  return Var(); }

// Params: (bool condition)
bool Language::fn_if(const vec<Var>& params){
  return false; } //!

void Language::fn_else(const vec<Var>& params){}

#endif

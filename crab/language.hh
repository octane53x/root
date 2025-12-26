// LANGUAGE DEPENDENCIES

#ifndef LANGUAGE_HH
#define LANGUAGE_HH

#include "var.hh"

struct Language {

  enum Control {
    USER,
    IF, ELSE, FOR, WHILE, BREAK, CONTINUE, RETURN,
    ADD, SUBTRACT, MULTIPLY, DIVIDE, MODULO,
    BITAND, BITOR, BITXOR, LSHIFT, RSHIFT };

  Var process_fn(const Control fn, const vec<Var>& params);
  bool fn_if(const vec<Var>& params);
  void fn_else(const vec<Var>& params); };

Var Language::process_fn(const Control fn, const vec<Var>& params){
  if(fn == USER)
    err("Language.process_fn", "called with USER code");
  switch(fn){
  case IF:
    return Var(fn_if(params));
  case ELSE:
    fn_else(params);
    return Var();
  //! more
  default:
    err("Language.process_fn", "control fn not handled");
    // Unreachable
    return Var(); } }

bool Language::fn_if(const vec<Var>& params){
  return false; } //!

void Language::fn_else(const vec<Var>& params){}

#endif

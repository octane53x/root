// LANGUAGE DEPENDENCIES

#ifndef LANGUAGE_HH
#define LANGUAGE_HH

#include "var.hh"

struct Language {

  // Function type
  enum Control {
    INVALID,
    // User-defined in executed script
    USER,
    // Handled by engine
    IF, ELSE, FOR, WHILE, BREAK, CONTINUE, RETURN,
    TYPENAME, EXIT, PRINT,
    // Block operations
    AND, OR, NOT, EQUAL, LESS, GREATER,
    ADD, SUBTRACT, MULTIPLY, DIVIDE, MODULO,
    BITAND, BITOR, BITXOR, BITINV, LSHIFT, RSHIFT };

  Var process_fn(const Control fn, const vec<Var>& params);
  bool fn_typename(const vec<Var>& params); };

Var Language::process_fn(const Control fn, const vec<Var>& params){
  if(fn == TYPENAME){
    //!
  }
  //! more
  err("Language.process_fn", "control fn not handled");
  // Unreachable
  return Var(); }

#endif

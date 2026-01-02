// LANGUAGE DEPENDENCIES

#ifndef DRIVER_HH
#define DRIVER_HH

#include "var.hh"

struct Driver {

  // Function type
  enum Control {
    INVALID,
    // User-defined in executed script
    USER, DECL,
    // Handled by engine
    IF, ELSE, FOR, WHILE, BREAK, CONTINUE, RETURN,
    TYPENAME, EXIT, PRINT,
    // Block operations
    AND, OR, NOT, EQUAL, LESS, GREATER,
    ADD, SUBTRACT, MULTIPLY, DIVIDE, MODULO,
    BITAND, BITOR, BITXOR, BITINV, LSHIFT, RSHIFT };

  Var process_fn(const Control fn, const vec<Var>& params);
  bool fn_typename(const vec<Var>& params); };

// System function
Var Driver::process_fn(const Control fn, const vec<Var>& params){
  if(fn == TYPENAME){
    //!
  }
  //! more
  err("Driver.process_fn", "control fn not handled");
  // Unreachable
  return Var(); }

#endif

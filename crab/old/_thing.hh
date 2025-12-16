// _THING

#ifndef _THING_HH
#define _THING_HH

#include "var.hh"

// Requirements for structures, functions, and pointers
struct _Thing : virtual Var {

  // Variable name
  Name name;
  // Variable type
  Type type;
  // Unique object instance identifier
  ID id;
  // Next ID to assign
  static ID next_id;
  // Containing object
  Var* ctr;

  _Thing();

};

// Set default member state
_Thing::_Thing():

#endif

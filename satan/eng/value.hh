// VALUE / VAL

#ifndef VALUE_HH
#define VALUE_HH

//#include "status.hh"
//#include "number.hh"
#include "thing.hh" //!

struct Value /*: virtual Status*/ {

  enum constant {EQUAL, NOT_EQUAL};

  constant cdata;
  //Num ndata;

  Value(const constant c);

};

Value::Value(const constant c): cdata(c) {}

Val Thing::compare(const Thing& o) const {
  return (*type == *o.type) ? Val::EQUAL : Val::NOT_EQUAL; }

#endif

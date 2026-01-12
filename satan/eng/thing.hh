// THING

#ifndef THING_HH
#define THING_HH

#include "../../core/util.hh"

struct Type;
typedef struct Value Val;
typedef struct Function Fn;

// Base class for everything
struct Thing {

  Type* type;
  umap<str, Fn> fns;

  // Defined in type.hh
  Thing();

  // Defined in type.hh
  virtual void register_type();
  // Defined in value.hh
  virtual Val compare(const Thing& o) const; };

struct Compare : virtual Function

#endif

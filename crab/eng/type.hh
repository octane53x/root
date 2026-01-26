// TYPE

#ifndef TYPE_HH
#define TYPE_HH

struct Type {

  // Identifier
  str name;
  // Direct parents
  uset<Type*> bases;
  // Member variables without address
  uset<Var> vars;
  // Member functions
  uset<Fn*> fns; };

#endif

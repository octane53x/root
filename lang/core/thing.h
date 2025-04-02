// thing.hh

//#include "incl.hh"
#include "base.hh"
#include "typ.hh"
#include "alloc.hh"

struct thing {

  _num id;
  static _num next_id;
  typ type;
  _str name;

  thing(){
    id = get_id();
    type = typ("thing");
    name = type.to_str() + id.to_str();
  }

  _num get_id(){
    if(!next_id) next_id = 1;
    return next_id++;
  }

  thing* clone(){
    thing* r = (thing*)alloc(sizeof(thing));
    r->id = get_id();
    r->type = type;
    r->name = name;
    return r;
  }
};
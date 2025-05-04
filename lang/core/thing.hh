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

  bool is_type(const char* _t){
    _str t = _str(_t);
    if(t == type.to_str()) return true;
    else if(t == "void" || type.to_str() == "void")
      return false;
    
    std::queue<typ> q;
    for(typ& u : type.parents)
      q.push(u);
    while(!q.empty()){
      typ u = q.front();
      if(u.to_str() == t) return true;
      for(typ& v : u.parents)
        q.push(v);
      q.pop();
    }
    return false;
  }

  thing* clone(){
    thing* r = (thing*)alloc(sizeof(thing));
    r->id = get_id();
    r->type = type;
    r->name = name;
    return r;
  }
};
// THING

#ifndef THING_HH
#define THING_HH

#include "util.hh"

struct typ {
  str name;
  vec<typ> parents;
  typ(): name("void"){}
  typ(const char* s): name(s){}
  typ& operator=(const char* s){
    name = str(s);
    return *this; } };

struct thing {
  llu id;
  static llu next_id;
  /*! static */ typ type;
  str name;

  thing(){
    type = "thing";
    id = get_id();
    /*! name = type.name + str(id); */ }

  void validate(){
    if(type.parents.empty()) type.parents.pb(typ("thing"));
    if(id < 1) id = get_id(); }

  llu get_id(){ return next_id++; }

  bool is_type(const char* _t){
    str t = str(_t);
    if(t == type.name) return true;
    else if(t == str("void") || type.name == str("void"))
      return false;
    std::queue<typ> q;
    for(typ& u : type.parents)
      q.push(u);
    while(!q.empty()){
      typ u = q.front();
      if(u.name == t) return true;
      for(typ& v : u.parents)
        q.push(v);
      q.pop(); }
    return false; }

  //!
  llu hash(){ return id; }

  thing* clone(){
    thing* r = new thing;
    r->id = get_id();
    r->type = type;
    r->name = name;
    return r; } };

llu thing::next_id = 1;

#endif

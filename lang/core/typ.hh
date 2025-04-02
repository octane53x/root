// typ.hh

#include <vector>

//#include "incl.hh"
#include "base.hh"

struct typ {

  _str name;
  std::vector<typ> parents;

  typ(): name("void"){}
  typ(const char* s): name(s){}
};
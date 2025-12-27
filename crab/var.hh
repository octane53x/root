// VARIABLE

#ifndef VAR_HH
#define VAR_HH

#include "type.hh"
#include "allocator.hh"

typedef uchar Char;
typedef llu Block;

const size_t
    SIZE_CHAR = 8,
    SIZE_BLOCK = 64;

struct Var {

  Type type;
  void* addr;
  str name;
  // Key = var name
  umap<str, Var> members;
  static Allocator* allocator;

  Var();
  Var(const str& _type, const Char val);
  Var(const str& _type, const Block val);

  void deallocate(); };

Var::Var(): addr(NULL) {}

Var::Var(const str& _type, const Char val): type(_type), name("") {
  addr = allocator->allocate(SIZE_CHAR);
  *((Char*)addr) = val; }

Var::Var(const str& _type, const Block val): type(_type), name("") {
  addr = allocator->allocate(SIZE_BLOCK);
  *((Block*)addr) = val; }

void Var::deallocate(){
  //! recursively deallocate members
}

#endif

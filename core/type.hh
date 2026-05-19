// TYPE

//! DISCONNECTED

#ifndef TYPE_HH
#define TYPE_HH

#include "util.hh"

struct str;
template <typename T>
struct list;

struct TypeMgr {

  // Next id for new type
  ID next_id;
  // Type name container
  umap<ID, str> types;
  // Inheritance tree
  umap<ID, uset<ID> > ancestry;

  // Constructor
  TypeMgr();

  // Add a new type
  // Defined in core.hh
  void add(const str& name, const list<str>& parents); };

// Exists to hold TypeMgr, so there is not one per type
struct _type {

  // Type manager
  static TypeMgr type_mgr; };

template <typename T>
struct type : virtual _type {

  // Pointer to type name held by TypeMgr
  static inline ID type_id = 0;

  void set_type(const str& name, const list<str>& parents); };

TypeMgr::TypeMgr():
  next_id(1) {}

template <typename T>
void type<T>::set_type(const str& name, const list<str>& parents){
  type_id = type_mgr.add(name, parents); }

#endif

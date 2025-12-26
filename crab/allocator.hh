// MEMORY ALLOCATOR

#ifndef ALLOCATOR_HH
#define ALLOCATOR_HH

#include "../core/util.hh"

const size_t
    ALLOCATION_MIN = (size_t)1024 * 1024 * 8, // 1 MB
    ALLOCATION_MAX = (size_t)1024 * 1024 * 1024 * 8; // 1 GB

struct Allocator {

  int total_memory;
  void* top; //! set of addrs

  void init();
  void* allocate(size_t size); };

void Allocator::init(){}

void* Allocator::allocate(size_t size){
  return NULL; }

#endif

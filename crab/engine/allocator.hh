// MEMORY ALLOCATOR

#ifndef ALLOCATOR_HH
#define ALLOCATOR_HH

#include "../../core/util.hh"

const size_t
    ALLOCATION_MIN = (size_t)1024 * 1024 * 8, // 1 MB
    ALLOCATION_MAX = (size_t)1024 * 1024 * 1024 * 8; // 1 GB

struct Allocator {

  size_t total_memory, used_memory;
  umap<void*, size_t> chunks;
  mmap<size_t, void*> free;

  void init();
  void* allocate(const size_t size);
  void deallocate(void* const addr, const size_t size);
  void sys_alloc(const size_t size);
  void defrag(); };

void Allocator::init(){
  sys_alloc(ALLOCATION_MIN);
  used_memory = 0; }

// Size in bits
void* Allocator::allocate(const size_t size){
  const str fn = "Allocator.allocate";
  // Cannot allocate
  assert(size > ALLOCATION_MAX - used_memory, fn, "out of memory");
  assert(size % sizeof(uchar) == 0, fn, "must allocate a multiple of "
      + to_string(sizeof(uchar)) + " bits (for now)");

  map<size_t, void*>::iterator it = free.end();
  --it;
  // No consecutive free space available
  if(size > it->first){
    // Must defrag
    if(size > ALLOCATION_MAX - total_memory){
      defrag();
      assert(size <= ALLOCATION_MAX - used_memory, fn,
          "defrag allocated additional memory");
    // Otherwise malloc new chunk
    }else
      sys_alloc(size);
    // Ensure available space
    it = free.end();
    --it;
    assert(size <= it->first, fn, "failed to provide enough memory"); }

  // Allocate
  it = free.lower_bound(size);
  size_t block_size = it->first;
  void* addr = it->second;
  free.erase(it);
  free.insert({block_size - size,
      (void*)((uchar*)addr + size / sizeof(uchar*))});
  used_memory += size;
  return addr; }

void Allocator::deallocate(void* const addr, const size_t size){
  //! algorithm
}

void Allocator::sys_alloc(const size_t size){
  void* addr = malloc(size);
  chunks[addr] = size;
  total_memory += size;
  free.insert({size, addr}); }

void Allocator::defrag(){
  //! algorithm
}

#endif

// OBJECT UTILITY

#ifndef OBJ_HH
#define OBJ_HH

#include "def.hh"

// Return whether a vector contains an element O(N)
template <typename T>
bool contains(const vec<T>& v, const T& item){
  for(int i = 0; i < v.size(); ++i)
    if(v[i] == item) return true;
  return false; }

// Return whether an unordered set contains an element O(logN)
template <typename T>
bool contains(const uset<T>& u, const T& item){
  return u.find(item) != u.end(); }

// Return whether an unordered map contains an element O(logN)
template <typename K, typename V>
bool contains(const umap<K, V>& u, const K& item){
  return u.find(item) != u.end(); }

#endif

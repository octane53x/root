// LIST

#ifndef LIST_HH
#define LIST_HH

#include <vector> //! repl
#include "thing.hh"

struct str;

template <typename T>
struct list : virtual thing {

  // Internal data
  vector<T> data;

  // Constructors
  list();

  // Operators
  list<T>& operator=(const list<T>& o);
  T& operator[](const int i);

  // Convert to string
  // Defined in str.hh
  virtual str to_str() const;
  // Ensure valid state
  virtual void _validate(const str& fn);

  // True if no contents
  bool empty() const;
  // Number of elements in list
  int size() const;
  // Return reference to element at index
  T& at(const int i) const;

  // Delete all contents
  void clear();
  // Add element to end
  void add(const T& e);
  // Insert element at index
  void insert(const T& e, const int i);
  // Remove element at index
  void remove(const int i); };

template <typename T>
list<T>::list(){}

template <typename T>
list<T>& list<T>::operator=(const list<T>& o){
  data = o.data; }

template <typename T>
T& list<T>::operator[](const int i){
  return data[i]; }

template <typename T>
void list<T>::_validate(const str& fn){
  thing::validate(fn); }

template <typename T>
bool list<T>::empty() const {
  return data.empty(); }

template <typename T>
int list<T>::size() const {
  return data.size(); }

template <typename T>
T& list<T>::at(const int i) const {
  assert(i >= 0 && i < size(), "list.at", "OOB");
  return data[i]; }

template <typename T>
void list<T>::clear(){
  data.clear(); }

template <typename T>
void list<T>::add(const T& e){
  data.push_back(e); }

template <typename T>
void list<T>::insert(const T& e, const int i){ //! O(N)
  assert(i >= 0 && i <= size(), "list.insert", "OOB");
  data.insert(e, i); }

template <typename T>
void list<T>::remove(const int i){ //! O(N)
  assert(i >= 0 && i < size(), "list.remove", "OOB");
  data.erase(data.begin() + i); }

#endif

// LIST

#ifndef LIST_HH
#define LIST_HH

#include <vector> //! repl
#include "block.hh"

struct Str;

// Container of sequential elements
template <typename T>
struct List : virtual Thing {

  // Internal data
  std::vector<T> data;

  // Constructors
  List();

  // Operators
  List<T>& operator=(const List<T>& o);
  T& operator[](const int i);

  // Convert to string
  // Defined in conv.hh
  virtual Str to_str() const;
  // Ensure valid state
  virtual void _validate(const Str& fn);

  // True if no contents
  bool empty() const;
  // Number of elements in List
  int size() const;
  // Return reference to element at index
  T& at(const int i) const;

  // Delete all contents
  void clear();
  // Add element to end
  void push(const T& e);
  // Remove element at end
  void pop();
  // Insert element at index
  void insert(const T& e, const int i);
  // Remove element at index
  void remove(const int i); };

template <typename T>
List<T>::List(){}

template <typename T>
List<T>& List<T>::operator=(const List<T>& o){
  data = o.data; }

template <typename T>
T& List<T>::operator[](const int i){
  return data[i]; }

template <typename T>
void List<T>::_validate(const Str& fn){
  Thing::validate(fn); }

template <typename T>
bool List<T>::empty() const {
  return data.empty(); }

template <typename T>
int List<T>::size() const {
  return data.size(); }

template <typename T>
T& List<T>::at(const int i) const {
  assert(i >= 0 && i < size(), "List.at()", "OOB");
  return data[i]; }

template <typename T>
void List<T>::clear(){
  data.clear(); }

template <typename T>
void List<T>::push(const T& e){
  data.push_back(e); }

template <typename T>
void List<T>::pop(){
  data.pop_back(); }

template <typename T>
void List<T>::insert(const T& e, const int i){ //! O(N)
  assert(i >= 0 && i <= size(), "List.insert()", "OOB");
  data.insert(e, i); }

template <typename T>
void List<T>::remove(const int i){ //! O(N)
  assert(i >= 0 && i < size(), "List.remove()", "OOB");
  data.erase(data.begin() + i); }

#endif

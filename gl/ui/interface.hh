// APPLICATION INTERFACE

#ifndef INTERFACE_HH
#define INTERFACE_HH

#include "../../core/util.hh"

typedef bool (*InputFn)(const KeyEvent&);

// Key mapping support
struct Interface {

  // Route inputs here
  Interface* focus;
  // Map of key to function
  umap<str, InputFn> keymap;

  virtual void map_fns() = 0;

  bool input(const KeyEvent& ke); };

// Route the key to the correct function
// Return whether the key performed an action
bool Interface::input(const KeyEvent& ke){
  bool action = false;
  if(contains(keymap, ke.key))
    action = keymap[ke.key](ke);
  if(!action && focus)
    action = focus->input(ke);
  return action; }

#endif

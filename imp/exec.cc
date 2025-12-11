// Execute console Impact on any OS

#include "fn_input.hh"

// Main function called on application execution
// Called by: OPERATING SYSTEM
int main(){
  debug_init(time_exec = time(NULL));
  imp.UI_MODE = Impact::UI_CONSOLE;
  imp.init();
  imp.run();
  return 0; }

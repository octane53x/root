// Execute console Impact on any OS

#include "global.hh"
#include "fn_input.hh"

// Main function called on application execution
// Called by: OPERATING SYSTEM
int main(){
  time_exec = clock();
  UI_MODE = UI_CONSOLE;
  imp.init();
  imp.run();
  return 0; }

// ABSTRACT SYSTEM

// Throwing state errors instead of handling smoothly helps ensure the quality
// of the engine. For example, if init is called twice, an error is thrown
// instead of just preventing a second call because it could mean a bigger
// logic error in the design. At this level, it really matters that operations
// are 100% sound. Almost all objects inherit system so errors here are worse.
// This way more errors are exposed in testing instead of remaining undetected.

#ifndef SYSTEM_HH
#define SYSTEM_HH

#include "thing.hh"

// Dynamic thing, having state and thus requiring storage.
// thing is used for objects created so frequently they cannot afford to occupy
// more than minimal memory. Inherit system otherwise.
struct system : virtual thing {

  bool initialized, active;
  str type;

  system();
  virtual ~system() = 0; //

  virtual void validate(str func);
  virtual void init();
  virtual void run();
  virtual void stop();
  virtual void update(double ms); };

// Set default member state
system::system(): type("system"), initialized(false) {}

// Implemented to avoid linker error
system::~system(){}

// Ensure valid state
void system::validate(str func){
  thing::validate(func);
  assert(type != "", "type is empty");
  assert(!(!initialized && active), "system unintialized but active"); }

// Call at the beginning of derived class init()
// Usually create and call init_members first, passing needed parameters
void system::init(){
  if(initialized) err(str("Called ")+type+str(".init again"));
  initialized = true; }

// Call at the beginning of derived class run()
void system::run(){
  if(!initialized) err(str("Called ")+type+str(".run before init"));
  if(active) err(str("Called ")+type+str(".run when running"));
  active = true; }

// Call at the beginning of derived class stop()
void system::stop(){
  if(!initialized) err(str("Called ")+type+str(".stop before init"));
  if(!active) err(str("Called ")+type+str(".stop when not running"));
  active = false; }

// An object cannot continuously update, because other tasks must be processed.
// The engine is recommended to operate a loop of (update -> output -> repeat).
// Call at the beginning of derived class update()
// Set last_update to clock() at the end of the update
void system::update(){
  if(!initialized) err(str("Called ")+type+str(".update before init"));
  if(!active) err(str("Called ")+type+str(".update when not running")); }

#endif

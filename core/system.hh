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

  // Last time the system was updated
  clock_t last_update;
  // Whether init has been called
  bool initialized;
  // Whether it is running
  bool active;
  // Whether it has been updated
  bool updated;
  // Unique id used as a map key for efficient storage
  llu id;
  // Next id to assign to a new system
  static llu next_id;
  // Class type name, set in derived constructors
  str type;

  system();
  virtual ~system() = 0; // Make object abstract

  virtual void validate(const str& func);
  virtual str to_str() const;
  virtual void init();
  virtual void run();
  virtual void stop();
  virtual void update(const double ms);

  llu new_id(); };

// Start the id numbering at 1
llu system::next_id = 1;

// Set default member state
system::system(): type("system"), initialized(false), active(false) {}

// Implemented to avoid linker error
system::~system(){}

// Ensure valid state
void system::validate(const str& func){
  assert(type != "", func, "type is empty");
  assert(!(!initialized && active), func, "system unintialized but active"); }

// Convert to string
str system::to_str() const {
  return type; }

// Call at the beginning of derived class init()
// Usually create and call init_members first, passing needed parameters
void system::init(){
  if(initialized) err(type+".init", "called twice");
  initialized = true;
  id = new_id();
  last_update = clock(); }

// Call at the beginning of derived class run()
void system::run(){
  if(!initialized) err(type+".run", "called before init");
  if(active) err(type+".run", "called when running");
  active = true; }

// Call at the beginning of derived class stop()
void system::stop(){
  if(!initialized) err(type+".stop", "called before init");
  if(!active) err(type+".stop", "called when not running");
  active = false; }

// Call at the beginning of derived class update()
// Set updated to true if it is determined that the object updated
// If so, set last_update to clock() at the end of the update
void system::update(const double ms){
  if(!initialized) err(type+".update", "called before init");
  if(!active) err(type+".update", "called when not running"); }

// Return a unique object id
llu system::new_id(){
  return next_id++; }

#endif

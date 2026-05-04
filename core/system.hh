// ABSTRACT SYSTEM

#ifndef SYSTEM_HH
#define SYSTEM_HH

#include "thing.hh"

// Initialization and update support
struct system : virtual thing {

  // Whether init has been called
  bool initialized;
  // Whether the system has changes to be processed
  bool updated;
  // Time of last update
  clock_t last_update;

  // Constructors
  system();
  // Virtual destructor to enforce abstraction
  virtual ~system() = 0;

  // Ensure valid state
  virtual void _validate(const str& fn);
  // Initialize state
  virtual void _init();
  void init();
  // Update system
  virtual void _update();
  void update(); };

system::system():
  initialized(false), updated(false), last_update(0) {}

system::~system(){}

void system::_validate(const str& fn){
  assert(!((updated || last_update > 0) && !initialized),
      fn, "update before init"); }

void system::_init(){}

void system::init(){
  assert(!initialized, "system.init", "already initialized");
  _init();
  initialized = true; }

void system::_update(){}

void system::update(){
  assert(initialized, "system.update", "not initialized");
  _update();
  updated = true;
  last_update = clock(); }

#endif

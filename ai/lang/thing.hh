// THING

#ifndef THING_HH
#define THING_HH

// Base structure inherited by all
struct Thing : virtual Var {

  // Variable name
  str name;
  // Variable type
  Type type;
  // Unique object instance identifier
  ID id;
  // Next ID to assign
  static ID next_id;
  // Alternative location of data (could be a chain)
  Var* ptr;
  // Containing object
  Thing* ctr;
  // Member data
  umap<str, Var*> members; // Key by name

  Thing();
  Thing(const Type& _type, const str& _name);

  Variable* operator()(const str& _name);

  virtual str to_str() const;
  virtual str get_state() const;
  virtual void load_state(const str& state);

  virtual void validate();
  virtual void init(const vec<Var*>& params);

  void err(const str& msg) const;
  void assert(const bool cond, const str& msg) const;

  ID new_id();
  void add(Var* member); };

// Start IDs at 1
ID Thing::next_id = 1;

// Set default member state
Thing::Thing(): prim(false), name(""), type("Thing"), ptr(NULL), ctr(NULL) {
  id = new_id();
  reg.add(type, {"Var"}); }

// Construct with type, name, and container
Thing::Thing(const str& _name, const Thing* _ctr): Thing() {
  name = _name, ctr = _ctr; }

// Get a member by name
Variable* Thing::operator()(const str& var_name){
  return members[var_name]; }

// Convert to readable string
str Thing::to_str() const {
  return type+" "+name+": "+to_string(id); }

// Convert to writable state to save
str Thing::get_state() const { return ""; }

// Load state from saved string
void Thing::load_state(const str& state){}

// Ensure valid state
void Thing::validate(){
  if(ctr != NULL)
    ctr->validate();
  assert(name != "", "name is empty");
  assert(type != "", "type is empty");
  assert(id > 0, "id is zero"); }

// Required implementation
void Thing::init(){}

// Throw an error
void Thing::err(const str& msg) const {
  //! Print chain of containers
  printf(("ERR: "+type+" "+name+": "+msg+"\n").c_str());
  exit(0); }

// Check a condition and error if false
void Thing::assert(const bool cond, const str& msg) const {
  if(!cond) err(msg); }

// Assign the next ID
ID Thing::new_id(){
  return next_id++; }

#endif

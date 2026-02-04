// CRAB ENGINE

#ifndef ENGINE_HH
#define ENGINE_HH

#include "util.hh"
#include "fn.hh"

struct Engine {

  // Current line number
  int nline;
  // Expected number of leading spaces for the current line
  int indent;
  // Number of encompassing control flow blocks
  int in_blocks;
  // Encompassing object definition
  Type* enc_obj;
  // Encompassing function definition
  Fn* enc_fn;
  // Declared variables
  umap<str, Var> vars;
  // Declared variables organized by scope for fast deletion on descope
  umap<llu, uset<Var*> > scope_table;

  // Compile errors thus far
  vec<str> errors;

  void init();

  bool compile(const str& file);
  void parse_line(str line);
  void parse_instr(vec<str> toks);
  Type* parse_value(vec<str> toks);

  void run(); };

// Initialize engine
void Engine::init(){
  indent = in_blocks = 0;
  Type type;
  type.name = "___main";
  enc_obj = &(types[type.name] = type);
  enc_fn = NULL; }

// Analyze code for errors
bool Engine::compile(const str& file){
  if(!exists(file)){
    errors.pb("ERR: Cannot access file " + file);
    return false; }
  vec<str> text = file_to_text(file);
  for(nline = 1; nline <= text.size(); ++nline)
    parse_line(text[nline - 1]);
  //! If no main fn, error
  return errors.empty(); }

// Execute the script
void Engine::run(){}

#endif

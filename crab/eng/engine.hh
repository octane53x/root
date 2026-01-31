// CRAB ENGINE

#ifndef ENGINE_HH
#define ENGINE_HH

#include "util.hh"
#include "fn.hh"

struct Engine {

  // PARSING

  // Expected number of leading spaces for the current line
  int indent;
  // Number of encompassing control flow blocks
  int in_blocks;
  // Encompassing object definition
  Type* enc_obj;
  // Encompassing function definition
  Fn* enc_fn;

  // ACCESS

  // Declared types thus far
  umap<str, Type> types;
  // Declared functions thus far
  umap<str, Fn> fns;
  // Declared variables
  umap<str, Var> vars;
  // Declared variables organized by scope for fast deletion on descope
  umap<llu, uset<Var*> > scope_table;

  // Compile errors thus far
  vec<str> errors;

  void init();

  bool compile(const str& file);
  void parse_line(str line, const int nline);
  Type* parse_value(vec<str> toks, const int nline);

  void run(); };

// Initialize engine
void Engine::init(){
  indent = in_blocks = 0;
  enc_obj = NULL;
  enc_fn = NULL; }

// Analyze code for errors
bool Engine::compile(const str& file){
  if(!exists(file)){
    errors.pb("ERR: Cannot access file " + file);
    return false; }
  vec<str> text = file_to_text(file);
  for(int n = 0; n < text.size(); ++n)
    parse_line(text[n], n + 1);
  //! If no main fn, error
  return errors.empty(); }

// Execute the script
void Engine::run(){}

#endif

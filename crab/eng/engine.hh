// CRAB ENGINE

#ifndef ENGINE_HH
#define ENGINE_HH

#include "util.hh"
#include "fn.hh"

struct Engine {

  // Expected number of leading spaces for the current line
  int indent;
  // Compile errors thus far
  vec<str> errors;
  // Declared types thus far
  umap<str, Type> types;
  // Declared functions thus far
  umap<str, Fn> fns;
  // Declared variables
  umap<str, Var> vars;
  // Declared variables organized by scope for fast deletion on descope
  umap<llu, uset<Var*> > scope_table;

  void init();

  bool compile(const str& file);
  void process_line(const str& line);

  void run(); };

// Initialize engine
void Engine::init(){
  indent = 0; }

// Analyze code for errors
bool Engine::compile(const str& file){
  if(!exists(file)){
    errors.pb("ERR: Cannot access file " + file);
    return false; }
  vec<str> text = file_to_text(file);
  for(int n = 0; n < text.size(); ++n)
    process_line(text[n], n + 1);
  //! If no main fn, error
  return errors.empty(); }

// Analyze a single instruction
void Engine::process_line(str line, int nline){
  // Verify indentation
  bool good = true;
  for(int i = 0; i < indent; ++i)
    if(line[i] != ' '){
      good = false;
      break; }
  if(!good)
    errors.pb("ERR Line " + to_string(nline) + ": Incorrect indentation");

  while(1){
    // Delete leading spaces
    while(!line.empty() && line[0] == ' ')
      line = line.substr(1);

    // Get next token
    str tok = next_tok(line);
    if(tok == "")
      return;
    line = line.substr(tok.size());

    // Skip comment
    if(starts_with(tok, "//"))
      return;

    //!

    // Syntax error
    errors.pb("ERR Line " + to_string(nline) + ": Unrecognized syntax"); } }

// Execute the script
void Engine::run(){}

#endif

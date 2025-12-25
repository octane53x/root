// CRAB COMPILER

#ifndef COMPILER_HH
#define COMPILER_HH

#include "../../core/util.hh"

const str
    TAB = "<TAB>";

enum Instr { VAR_DECL, VAR_MOD, FN_DECL, FN_CALL, OBJ_DEF };

struct Compiler {

  struct File {
    bool vis;
    str name;
    vec<File*> sources;
    vec<str> code;
    vec<Instr> instr; };

  umap<str, File> files;

  void compile(const str& main_fname);
  File* load_file(const str& fname);
  void clean_code();
  void reset_files();

  // Defined in util
  bool is_upper(const char c) const;
  bool is_lower(const char c) const;
  bool is_digit(const char c) const;
  str next_tok(const str& line) const;
  str delete_tok(const str& line) const;
  str next_val(const str& line) const;
  str delete_val(const str& line) const;

  // Defined in other files
  void verify_syntax(const str& fname);
  void verify_elements(const str& fname);
  void verify_logic(const str& fname); };


// Compile code to executable
void Compiler::compile(const str& main_fname){
  load_file(main_fname);
  clean_code();
  // Check for code errors
  reset_files();
  verify_syntax(main_fname);
  reset_files();
  verify_elements(main_fname);
  reset_files();
  verify_logic(main_fname);

  // Process into engine components
  //! output

  // Compile into executable
  system("g++ assembler.cc -o main

// Load all included files
Compiler::File* Compiler::load_file(const str& fname){
  str func = "Compiler.load_file";
  if(!exists(fname))
    err(func, "file does not exist: " + fname);

  // Load file
  File file;
  file.name = fname;
  ifstream fs(file.name);
  str fline;
  while(getline(fs, fline))
    file.code.pb(fline);
  fs.close();

  // Harvest includes recursively
  for(const str& line : file.code){
    if(line.find("include ") == 0){
      int i = line.find("\"");
      if(i == str::npos || !(i > 7 && i < line.size() - 2))
        err(func, "bad include");
      for(int j = 8; j < i; ++j)
        if(line[j] != ' ')
          err(func, "bad include");
      int j = line.substr(i + 2).find("\"");
      if(j == str::npos || !(j < line.size()))
        err(func, "bad include");
      str f = line.substr(i + 1, j + 1);
      if(files.find(f) != files.end()) continue;
      file.sources.pb(load_file(f)); } }

  // Store file
  files[file.name] = file;
  return &files[file.name]; }

// Clean all files for processing
void Compiler::clean_code(){
  vec<str> code2;
  for(pair<str, File> p : files){
    File& file = p.second;
    for(str line : file.code){
      // Remove comments
      int i = line.find("//");
      if(i != str::npos)
        line = line.substr(0, i);
      // Remove includes
      if(line.find("include ") == 0)
        line = "";
      // Replace leading spaces with tab token
      while(line.size() >= 2 && line[0] == ' ' && line[1] == ' ')
        line = TAB + line.substr(2); }
    file.code = code2; } }

// Unvisit all files
void Compiler::reset_files(){
  for(pair<str, File> p : files)
    p.second.vis = false; }

#endif

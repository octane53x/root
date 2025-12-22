// CRAB COMPILER

#ifndef COMPILER_HH
#define COMPILER_HH

#include "../core/util.hh"

struct Compiler {

  struct File {
    str name;
    vec<File*> sources;
    vec<str> code; };

  umap<str, File> files;

  void compile(const str& main_fname);
  File* load_file(const str& fname); };

void Compiler::compile(const str& main_fname){
  load_file(main_fname);
  //!
}

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

#endif

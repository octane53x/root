// INCLUDER

#ifndef INCLUDER_HH
#define INCLUDER_HH

#include "file.hh"

struct Includer {

  void process_file(const str& fname, umap<str, File>* files); };

void Includer::process_file(const str& fname, umap<str, File>* files){}

#endif

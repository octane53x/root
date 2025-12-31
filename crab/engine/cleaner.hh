// CODE CLEANER

#ifndef CLEANER_HH
#define CLEANER_HH

#include "util.hh"
#include "file.hh"

struct Cleaner {

  void clean_files(umap<str, File>* files); };

//! Remove extra spaces & lines, compress continued lines
void Cleaner::clean_files(umap<str, File>* files){}

#endif

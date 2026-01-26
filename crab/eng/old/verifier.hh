// VERIFIER

#ifndef VERIFIER_HH
#define VERIFIER_HH

#include "util.hh"
#include "file.hh"

struct Verifier {

  void verify_files(const umap<str, File>& files); };

void Verifier::verify_files(const umap<str, File>& files){}

#endif

// LANGUAGE

#ifndef LANG_HH
#define LANG_HH

#include "../core/util.hh"

struct Type;
typedef pair<Type*, str> VarDecl;

const size_t
    SIZE_CHAR = 8,
    SIZE_BLOCK = 64;

// Object, function, or variable modifier
enum Mod {ABSTRACT, TEMPLATE, CONST, STATIC, VIRTUAL, FORCE, FINAL, REF};
const umap<str, Mod> TOK_TO_MOD =
    {{"abstract", ABSTRACT}, {"template", TEMPLATE}, {"const", CONST},
    {"static", STATIC}, {"virtual", VIRTUAL}, {"force", FORCE},
    {"final", FINAL}, {"&", REF}};

#endif

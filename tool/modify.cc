// MODIFY CODE

// Performs text modification operations on all files in root, recursive

#include "../core/util.hh"

struct mod {
  void replace_after(const str& dir, const str& after,
      const str& src, const str& dest);
} m;

// --------------------------------
// Enter operation here using m
// --------------------------------

// File targets
const vec<str> FILE_SUFFIXES = {".hh", ".cc", ".cpp"};

// Custom operation
void op(){
  m.replace_after(".", "assert", ",", ", func,");
}

// --------------------------------
// --------------------------------

// Returns whether the string ends with a given substring
bool ends_with(const str& s, const str& suf){
  if(suf.size() > s.size()) return false;
  bool good = true;
  for(int i = s.size()-1, j = suf.size()-1; j >= 0; --i, --j)
    if(s[i] != s[j]){ good = false; break; }
  return good; }

// Return the index of the first occurrence of substring in s, -1 if not found
int find(const str& s, const str& sub){
  for(int i = 0; i < s.size(); ++i){
    bool match = true;
    for(int j = 0; j < sub.size(); ++j){
      if(i+j == s.size()) break;
      if(s[i+j] != sub[j]){ match = false; break; } }
    if(match) return i; }
  return -1; }

// For all occurrences of "after", replace the next occurrence of src with dest
void mod::replace_after(const str& dir, const str& after,
    const str& src, const str& dest){
  print("Entering "+dir+str("\n"));

  // Look into the current directory
  vec<str> dirs, files;
  for(const auto& entry : directory_iterator(".")){
    str s = entry.path().string();
    if(is_directory(entry.status())){
      dirs.pb(s);
    }else{
      bool target = false;
      for(str suf : FILE_SUFFIXES)
        if(ends_with(s, suf))
          target = true;
      if(target)
        files.pb(s); } }

  for(const str& f : files){
    // Read the file
    str path = dir + str("/") + f;
    ifstream input(path);
    vec<str> lines;
    str line;
    while(getline(input, line))
      lines.pb(line);
    input.close();

    // Replace and write
    remove(path);
    ofstream output(path);
    for(str s : lines){
      int count = 0;
      while(1){
        int i = find(s, after);
        if(i == -1) break;
        str begin = s.substr(0, i + after.size());
        str rest = s.substr(i + after.size());
        int j = find(rest, src);
        if(j == -1) break;
        str mid = rest.substr(0, j);
        str end = rest.substr(j + src.size());
        ++count;
        s = begin + mid + dest + end; }
      print("  Made "+to_string(count)+" changes to "+path+"\n");
      output << s << endl; }
    output.close(); }

  // Recurse each subdirectory
  for(const str& d : dirs)
    replace_after(dir + str("/") + d, after, src, dest); }

int main(){
  op();
  return 0; }

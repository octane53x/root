// VERIFY SYNTAX

#ifndef SYNTAX_HH
#define SYNTAX_HH

#include "util.hh"

// Check for syntax errors
void Compiler::verify_syntax(const str& fname){
  str func = "Compiler.verify_syntax";
  File& file = files[fname];
  if(file.vis) return;
  file.vis = true;
  // Depth-first recursion to compile sources first
  for(File* f : file.sources)
    verify_syntax(f->name);

  // Check every line
  bool in_obj_def = false;
  bool in_fn_def = false;
  for(int il = 0; il < file.code.size(); ++il){
    str line = file.code[il];
    str ln = ": line " + to_string(il);
    if(line == "") continue;

    // Variable declaration
    str tok = next_tok(line);
    if(tok != "" && (is_upper(tok[0]) || tok[0] == '_')){
      // Verify type token
      for(int i = 1; i < tok.size() - 1; ++i)
        assert(is_upper(tok[i]) || is_lower(tok[i]) || is_digit(tok[i]),
            func, "invalid type" + ln);
      char c = tok.back();
      assert(is_upper(c) || is_lower(c) || is_digit(c) || c == '&' || c == '*',
          func, "invalid type" + ln);
      line = delete_tok(line);

      // Verify name token
      tok = next_tok(line);
      assert(tok != "" && is_lower(tok[0]) || tok[0] == '_',
          func, "invalid name" + ln);
      int i;
      for(i = 1; i < tok.size(); ++i){
        if(tok[i] == '(') break;
        assert(is_lower(tok[i]) || is_digit(tok[i]) || tok[i] == '_',
            func, "invalid name" + ln); }

      // Verify parameters
      if(i < tok.size() && tok[i] == '('){
        line = line.substr(i + 1);
        while(true){
          str val = next_val(line);
          if(val == ""){
            tok = next_tok(line);
            assert(tok == ")", func, "invalid parameters" + ln);
            break; }
          line = delete_val(line);
          tok = next_tok(line);
          if(tok == ")"){
            line = delete_tok(line);
            assert(line == "", func, "invalid declaration" + ln);
            break; }
          assert(tok == ",", func, "invalid parameters" + ln);
          line = line.substr(1); } }

      // Verify initialization

      file.instr.pb(VAR_DECL);
      continue; }

    // Variable modification


    // Function declaration


    // Function call


    // Object definition


    // Else error
    if(0) //!
      err(func, "invalid syntax: line " + to_string(il)); } }

#endif

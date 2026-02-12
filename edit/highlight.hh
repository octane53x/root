// SYNTAX HIGHLIGHTING

#ifndef HIGHLIGHT_HH
#define HIGHLIGHT_HH

#include "panel.hh"

void Panel::highlight_text(const int y0, const int yf){
  // Set default color
  while(text_color.size() < text.size())
    text_color.pb(vec<color>());
  color ct = cmd ? BAR_TEXT_COLOR : COLOR_CODE;
  for(int y = 0; y < y0; ++y)
    while(text_color[y].size() < text[y].size())
      text_color[y].pb(ct);
  for(int y = y0; y <= yf; ++y){
    text_color[y].clear();
    for(int x = 0; x < text[y].size(); ++x)
      text_color[y].pb(ct); }
  for(int y = yf + 1; y < text.size(); ++y)
    while(text_color[y].size() < text[y].size())
      text_color[y].pb(ct);
  if(cmd) return;

  if(file_type == CPP){
    bool in_comment_block = false, in_str = false, in_val = false,
        in_fn = false, in_preproc = false, in_params = false, in_for = false,
        in_obj_decl = false,
        ready_for_type = true, type_found = false, name_found = false;
    int in_obj = 0, braces = 0, parens = 0;
    ipoint pos(0, y0), type_pt, tok_pt, last_pt;
    str tok, last_tok, last_tok2;

    while(pos.y <= yf){
      // Next line
      if(pos.x >= text[pos.y].size()){
        if(in_preproc){
          in_preproc = false;
          ready_for_type = true; }
        ++pos.y;
        pos.x = 0;
        continue; }

      // Get next token
      if(!all_spaces(tok)){
        last_tok2 = last_tok;
        last_tok = tok;
        last_pt = tok_pt; }
      tok = next_tok(text[pos.y].substr(pos.x));
      tok_pt = pos;

      // Comment block
      if(in_comment_block){
        for(int i = pos.x; i < pos.x + tok.size(); ++i)
          text_color[pos.y][i] = COLOR_COMMENT;
        if(tok == "*/")
          in_comment_block = false;
        pos.x += tok.size();
        continue; }
      if(!in_str && tok == "/*"){
        in_comment_block = true;
        continue; }

      // String
      if(in_str){
        for(int i = pos.x; i < pos.x + tok.size(); ++i)
          text_color[pos.y][i] = COLOR_STRING;
        if(tok == "\"" && !(last_tok == "\\" && last_tok2 != "\\"))
          in_str = false;
        pos.x += tok.size();
        continue; }
      if(tok == "\"" && !(last_tok == "\\" && last_tok2 != "\\")){
        in_str = true;
        ready_for_type = false;
        text_color[pos.y][pos.x] = COLOR_STRING;
        ++pos.x;
        continue; }

      // Comment
      if(tok == "//"){
        while(pos.x < text[pos.y].size())
          text_color[pos.y][pos.x++] = COLOR_COMMENT;
        continue; }

      // Preprocessor
      if(tok == "#"){
        in_preproc = true;
        type_found = false;
        str t = next_tok(text[pos.y].substr(pos.x + 1));
        for(int i = pos.x; i < pos.x + t.size() + 1; ++i)
          text_color[pos.y][i] = COLOR_PREPROCESSOR;
        pos.x += t.size() + 1;
        continue; }

      // Keyword
      if(KEYWORDS.find(tok) != KEYWORDS.end()){
        if(tok == "struct" || tok == "class"){
          in_obj_decl = true;
          ++in_obj; }
        else if(tok == "for")
          in_for = true;
        color ct = (tok == "true" || tok == "false" || tok == "NULL")
            ? COLOR_BASE : COLOR_KEYWORD;
        for(int i = pos.x; i < pos.x + tok.size(); ++i)
          text_color[pos.y][i] = ct;
        pos.x += tok.size();
        continue; }

      // Inside function or brace
      if(tok == "{"){
        in_obj_decl = false;
        if(!in_val){
          type_found = false;
          name_found = false;
          ready_for_type = true; }
        if(last_tok == ")"){
          in_fn = true;
          in_params = false; }
        ++braces;
        ++pos.x;
        continue; }

      // End of function or brace
      if(tok == "}"){
        if(!in_val)
          ready_for_type = true;
        if(in_fn && braces == in_obj + 1)
          in_fn = false;
        if(in_obj > 0 && braces == in_obj)
          --in_obj;
        if(braces > 0)
          --braces;
        ++pos.x;
        continue; }

      // Enter paren
      if(tok == "("){
        if(last_tok == "for")
          ready_for_type = true;
        if(last_tok != "for" && last_tok != "if" && in_fn)
          in_val = true;
        else if(!in_fn){
          ready_for_type = true;
          if(last_tok != "for")
            in_params = true; }
        ++parens;
        ++pos.x;
        continue; }

      // Leave paren
      if(tok == ")"){
        if(parens > 0)
          --parens;
        if(parens == 0 && in_for)
          in_for = false;
        ++pos.x;
        continue; }

      // Val
      if(ends_with(tok, "=") && tok != "=="){
        in_val = true;
        ready_for_type = false;
        pos.x += tok.size();
        continue; }

      // End of instruction
      if(tok == ";"){
        in_val = type_found = name_found = in_obj_decl = false;
        ready_for_type = true;
        ++pos.x;
        continue; }

      // Next instruction, parameter, or value
      if(tok == ","){
        if(!name_found)
          type_found = false;
        if(parens == 0 || in_for)
          in_val = false;
        ready_for_type = in_params;
        ++pos.x;
        continue; }

      // Type scoping
      if(tok == "::"){
        str t = next_tok(text[last_pt.y].substr(last_pt.x));
        for(int i = last_pt.x; i < last_pt.x + t.size(); ++i)
          text_color[last_pt.y][i] = COLOR_BASE;
        pos.x += 2;
        continue; }

      // Type declaration
      if(in_obj_decl && type_or_name(tok)){
        for(int i = pos.x; i < pos.x + tok.size(); ++i)
          text_color[pos.y][i] = COLOR_TYPE;
        pos.x += tok.size();
        continue; }

      // Maybe type
      if(ready_for_type && type_or_name(tok)){
        str t = next_tok(text[pos.y].substr(pos.x + tok.size()));
        if(all_spaces(t) || t == "*" || t == "**" || t == "&" || t == "<"){
          type_found = true;
          type_pt = pos; }
        ready_for_type = false;
        pos.x += tok.size();
        continue; }

      // Templated type
      if(type_found && tok == "<"){
        bool found = true;
        int n = 0;
        ipoint p = pos, lc;
        while(1){
          str t = next_tok(text[p.y].substr(p.x));
          if(t != "<" && t != ">" && t != ","
              && !type_or_name(t) && !all_spaces(t)){
            found = false;
            break; }
          if(t == "<")
            ++n;
          if(t == ">"){
            --n;
            lc = p;
            if(n == 0) break; }
          p.x += t.size();
          if(p.x >= text[p.y].size()){
            ++p.y;
            p.x = 0; } }
        if(found){
          p = type_pt;
          while(p != lc){
            if(text[p.y][p.x] != '<' && text[p.y][p.x] != '>')
              text_color[p.y][p.x] = COLOR_TYPE;
            ++p.x;
            if(p.x >= text[p.y].size()){
              ++p.y;
              p.x = 0; } }
          pos = lc;
          ++pos.x;
          continue; } }

      // Var name
      if(type_found && !in_val && type_or_name(tok)){
        ready_for_type = false;
        name_found = true;
        str tn = next_tok(text[pos.y].substr(pos.x + tok.size()));
        if(all_spaces(tn))
          tn = next_tok(text[pos.y].substr(pos.x + tok.size() + tn.size()));
        color ct = (!in_fn && tn == "(") ? COLOR_FUNCTION : COLOR_NAME;
        str t = next_tok(text[type_pt.y].substr(type_pt.x));
        for(int i = type_pt.x; i < type_pt.x + t.size(); ++i)
          text_color[type_pt.y][i] = COLOR_TYPE;
        for(int i = pos.x; i < pos.x + tok.size(); ++i)
          text_color[pos.y][i] = ct;
        pos.x += tok.size();
        continue; }

      // Cancel type
      if(type_found && !in_val && !type_or_name(tok) && !all_spaces(tok)
          && tok != "*" && tok != "&"){
        type_found = false;
        pos.x += tok.size();
        continue; }

      // Skip other token
      pos.x += tok.size(); }

  }else if(file_type == PYTH){
    //!
  }else if(file_type == CRAB){
    //!
  } }

#endif

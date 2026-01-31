// CODE PARSING

#ifndef PARSE_HH
#define PARSE_HH

#include "engine.hh"

// Modifiers
const uset<str>
    OBJ_MOD = {"abstract"},
    FN_MOD = {"const"},
    VAR_MOD = {"const"},
    MEMBER_FN_MOD = {"const", "static", "virtual", "force", "final"},
    MEMBER_VAR_MOD = {"const", "static"};

// Analyze a single instruction
void Engine::parse_line(str line, const int nline){
  const str _err = "ERR Line " + to_string(nline) + ": ";

  // Verify indentation
  int spaces = 0;
  while(spaces < line.size() && line[spaces] == ' ')
    ++spaces;
  if((spaces & 1) || spaces > indent)
    errors.pb(_err + "Incorrect indentation");

  // Leave scope
  while(spaces < indent){
    if(in_blocks > 0)
      --in_blocks;
    else if(enc_fn != NULL)
      enc_fn = NULL;
    else if(enc_obj != NULL){
      if(enc_obj->ctr != NULL)
        enc_obj = enc_obj->ctr;
      else
        enc_obj = NULL;
    }else
      err("Engine.parse_line", "Negative indentation"); //! remove
    indent -= 2; }

  // Toss comment
  int k = line.find("//");
  if(k != str::npos)
    line = line.substr(0, k);

  // --------------------------------
  // Object Declaration
  // --------------------------------

  vec<str> toks = split_tok(line);
  int i;
  for(i = 0; i < toks.size() && toks[i] != "obj"; ++i);
  if(i < toks.size()){
    Type type;

    // Verify modifiers
    for(int j = 0; j < i; ++j){
      if(contains(OBJ_MOD, toks[j])){
        if(contains(type.mods, toks[j])){
          errors.pb(_err + "Modifier declared twice");
          return;
        }else
          type.mods.insert(toks[j]);
      }else{
        errors.pb(_err + "Invalid type modifier");
        return; } }

    // Verify type name
    if(++i >= toks.size()){
      errors.pb(_err + "Expected type name");
      return; }
    if(is_type(toks[i]))
      type.name = toks[i];
    else{
      errors.pb(_err + "Invalid type name");
      return; }

    // Expect colon otherwise forward declaration
    if(++i < toks.size()){
      if(toks[i] != ":"){
        errors.pb(_err + "Expected colon");
        return; }
    }else{
      types[type.name] = type;
      return; }

    // Verify base types
    if(++i < toks.size()){
      while(1){
        if(!is_type(toks[i])){
          errors.pb(_err + "Expected type name");
          return; }
        type.bases[toks[i]] = &types[toks[i]];
        if(++i >= toks.size()){
          errors.pb(_err + "Expected colon or comma");
          return; }
        if(toks[i] == ":")
          break;
        if(toks[i] != ","){
          errors.pb(_err + "Expected colon or comma");
          return; }
        if(++i >= toks.size()){
          errors.pb(_err + "Expected type name");
          return; } } }

    // Add type
    indent += 2;
    type.defined = true;
    type.ctr = enc_obj;
    enc_obj = &(types[type.name] = type); }

  // --------------------------------
  // Function Declaration
  // --------------------------------

  for(i = 0; i < toks.size() && toks[i] != "fn"; ++i);
  if(i < toks.size()){
    Fn fn;

    // Verify modifiers
    const uset<str>* MODS = (enc_obj == NULL) ? &FN_MOD : &MEMBER_FN_MOD;
    for(int j = 0; j < i; ++j){
      if(contains(*MODS, toks[j])){
        if(contains(fn.mods, toks[j])){
          errors.pb(_err + "Modifier declared twice");
          return;
        }else
          fn.mods.insert(toks[j]);
      }else{
        errors.pb(_err + "Invalid type modifier");
        return; } }

    // Verify containing type
    if(++i >= toks.size()){
      errors.pb(_err + "Expected function name");
      return; }
    if(is_type(toks[i])){
      if(i + 2 >= toks.size() || toks[i+1] != ":" || toks[i+2] != ":"){
        errors.pb(_err + "Expected double colon");
        return; }
      if(!contains(types, toks[i]) || !types[toks[i]].defined){
        errors.pb(_err + "Type not defined");
        return; }
      fn.ctr = &types[toks[i]];
      if((i += 3) >= toks.size()){
        errors.pb(_err + "Expected function name");
        return; } }

    // Verify function name
    if(is_name(toks[i]))
      fn.name = toks[i];
    else{
      errors.pb(_err + "Invalid function name");
      return; }

    // Ensure function is declared on type but not yet defined
    if(fn.ctr != NULL){
      if(!contains(fn.ctr->fns, fn.name)){
        errors.pb(_err + "Function not declared on type");
        return; }
      if(fn.ctr->fns[fn.name]->defined){
        errors.pb(_err + "Function already defined");
        return; } }

    // Expect parenthesis
    if(++i >= toks.size() || toks[i] != "("){
      errors.pb(_err + "Expected parenthesis");
      return; }

    // Verify parameters
    bool got_param = false;
    while(1){
      // Expect parenthesis or comma
      if(++i >= toks.size()){
        errors.pb(_err + "Expected parenthesis");
        return; }
      if(toks[i] == ")"){
        if(++i < toks.size()){
          if(toks[i] == ":")
            break;
          else{
            errors.pb(_err + "Expected colon");
            return; }
        // Declare function
        }else{
          fns[fn.name] = fn;
          if(enc_obj != NULL)
            enc_obj->fns[fn.name] = &fns[fn.name];
          return; } }
      if(got_param && toks[i] != ","){
        errors.pb(_err + "Expected parenthesis or comma");
        return; }

      // Get param
      if((toks[i] == "," && ++i >= toks.size()) || !is_type(toks[i])){
        errors.pb(_err + "Expected type");
        return; }
      if(!contains(types, toks[i]) || !types[toks[i]].defined){
        errors.pb(_err + "Parameter type not defined");
        return; }
      Var var;
      var.type = &types[toks[i]];
      if(++i >= toks.size() || !is_name(toks[i])){
        errors.pb(_err + "Expected variable name");
        return; }
      var.name = toks[i];
      fn.params.pb(var);
      got_param = true; }

    // Add function
    indent += 2;
    fn.defined = true;
    enc_fn = &(fns[fn.name] = fn);
    if(enc_obj != NULL){
      fn.ctr = enc_obj;
      enc_obj->fns[fn.name] = enc_fn; } }

  // --------------------------------
  // Operation / Control Flow
  // --------------------------------

  // Check for variable declaration modifiers
  const uset<str>* MODS = (enc_obj == NULL) ? &VAR_MOD : &MEMBER_VAR_MOD;
  Var var;
  for(i = 0; i < toks.size() && contains(*MODS, toks[i]); ++i){
    if(contains(var.mods, toks[i])){
      errors.pb(_err + "Modifier declared twice");
      return; }
    var.mods.insert(toks[i]); }

  if(i < toks.size()){
    // If statement
    if(toks[i] == "if"){
      //!
    }

    // For loop
    if(toks[i] == "for"){
      //!
    }

    // While loop
    if(toks[i] == "while"){
      //!
    }

    // Break or continue
    if(toks[i] == "break" || toks[i] == "continue"){
      //!
    }

    // Return
    if(toks[i] == "return"){
      //!
    }

    // Variable declaration
    if(is_type(toks[i])){
      if(!contains(types, toks[i]) || !types[toks[i]].defined){
        errors.pb(_err + "Variable type not defined");
        return; }
      var.type = &types[toks[i]]; }

    if(is_name(toks[i])){
      if(i + 1 >= toks.size()){
        // Variable declaration without assignment
        if(var.type != NULL){
          var.name = toks[i];
          vars[var.name] = var;
        }else
          errors.pb(_err + "Expected operation");
        return; }

      // Determine variable or function
      //!

      //! ensure fn or var existence

    // Syntax error
    }else{
      errors.pb(_err + "Invalid operation");
      return; } } }

// Analyze the value portion of an instruction and find its type
Type* parse_value(vec<str> toks, const int nline){
  const str _err = "ERR Line " + to_string(nline) + ": ";

  //!
}

#endif

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
  const str
      _fn = "Engine.process_line",
      _err = "ERR Line " + to_string(nline) + ": ";

  // Verify indentation
  int spaces = 0;
  while(spaces < line.size() && line[spaces] == ' ')
    ++spaces;
  if((spaces & 1) || spaces > indent)
    errors.pb(_err + "Incorrect indentation");

  // Leave scope
  while(spaces < indent){
    if(in_block > 0)
      --in_block;
    else if(enc_fn != NULL)
      enc_fn = NULL;
    else if(enc_obj != NULL){
      if(enc_obj->ctr != NULL)
        enc_obj = enc_obj->ctr;
      else
        enc_obj = NULL;
    }else
      err(_fn, "Negative indentation");
    indent -= 2; }

  // Toss comment
  int k = line.find("//");
  if(k != str::npos)
    line = line.substr(0, k);

  // Obj declaration
  vec<str> toks = split_tok(line);
  int i;
  for(i = 0; i < toks.size() && toks[i] != "obj"; ++i);
  if(i < toks.size()){
    Type type;

    // Verify modifiers
    for(int j = 0; j < i; ++j){
      if(OBJ_MOD.find(toks[j]) != OBJ_MOD.end()){
        if(type.mods.find(toks[j]) != type.mods.end()){
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
        type.bases.insert(&types[toks[i]]);
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

  // Function declaration
  for(i = 0; i < toks.size() && toks[i] != "fn"; ++i);
  if(i < toks.size()){
    Fn fn;

    // Verify modifiers
    const uset<str>* MODS = (enc_obj == NULL) ? &FN_MOD : &MEMBER_FN_MOD;
    for(int j = 0; j < i; ++j){
      if(MODS->find(toks[j]) != MODS->end()){
        if(fn.mods.find(toks[j]) != fn.mods.end()){
          errors.pb(_err + "Modifier declared twice");
          return;
        }else
          fn.mods.insert(toks[j]);
      }else{
        errors.pb(_err + "Invalid type modifier");
        return; } }

    // Verify function name
    if(++i >= toks.size()){
      errors.pb(_err + "Expected function name");
      return; }
    if(is_name(toks[i]))
      fn.name = toks[i];
    else{
      errors.pb(_err + "Invalid function name");
      return; }

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
        }else{
          fns[fn.name] = fn;
          return; } }
      if(got_param && toks[i] != ","){
        errors.pb(_err + "Expected parenthesis or comma");
        return; }

      // Get param
      if((toks[i] == "," && ++i >= toks.size()) || !is_type(toks[i])){
        errors.pb(_err + "Expected type");
        return; }
      if(types.find(toks[i]) == types.end() || !types[toks[i]].defined){
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
    fn.ctr = enc_obj;
    enc_fn = &(fns[fn.name] = fn); }

  //! VAR
  //! OP
  //! Control flow: IF FOR WHILE CONTINUE BREAK RETURN
}

#endif

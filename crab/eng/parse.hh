// CODE PARSING

// If X already declared, cannot declare again
// If X already defined, cannot declare OR define again
// X = TYPE: within all containing types
// X = FN or VAR: just within current type

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
void Engine::parse_line(str line){
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
    else
      enc_obj = enc_obj->ctr;

    // Remove vars at scope
    for(Var* var : scope_table[indent])
      vars.erase(var->name);
    scope_table.erase(indent);
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
    if(!is_type(toks[i]))
      errors.pb(_err + "Invalid type name");
      return; }
    type.name = toks[i];

    // Declare type
    if(++i >= toks.size()){
      Type* t = enc_obj->get_type(type.name);
      if(t == NULL)
        enc_obj->types[type.name] = type;
      else
        errors.pb(_err + "Type already declared");
      return; }

    // Expect colon
    if(toks[i] != ":"){
      errors.pb(_err + "Expected colon");
      return; }

    // Verify base types
    if(++i < toks.size()){
      while(1){
        if(!is_type(toks[i])){
          errors.pb(_err + "Expected type name");
          return; }
        Type* base = enc_obj->get_type(toks[i]);
        if(base == NULL || !base->defined){
          errors.pb(_err + "Base type not defined");
          return; }
        type.bases[toks[i]] = base;
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

    // Ensure not already defined
    Type* t = enc_obj->get_type(type.name);
    if(t != NULL && t->defined){
      errors.pb(_err + "Type already defined");
      return; }

    // Define type
    indent += 2;
    type.defined = true;
    type.ctr = enc_obj;
    enc_obj = &(enc_obj->types[type.name] = type); }

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
      if(enc_obj->name != "___main"){
        errors.pb(_err + "Scoped function definition inside object");
        return; }
      if(!contains(enc_obj->types, toks[i])
          || !enc_obj->types[toks[i]].defined){
        errors.pb(_err + "Type not defined");
        return; }
      Type* t = enc_obj->types[toks[i]];
      while(1){
        if(++i >= toks.size() || toks[i] != "::"){
          errors.pb(_err + "Expected double colon");
          return; }
        if(++i >= toks.size()){
          errors.pb(_err + "Expected function name");
          return; }
        if(is_name(toks[i]))
          break;
        if(!is_type(toks[i])){
          errors.pb(_err + "Expected function name");
          return; }
        if(!contains(t->types, toks[i]) || !t->types[toks[i]].defined){
          errors.pb(_err + "Type not defined");
          return; }
        fn.ctr = t = &t->types[toks[i]]; } }

    // Verify function name
    if(is_name(toks[i])){
      if(contains(enc_obj->fns, toks[i]) && enc_obj->fns[toks[i]].defined){
        errors.pb(_err + "Function already defined");
        return; }
      fn.name = toks[i];
    }else{
      errors.pb(_err + "Invalid function name");
      return; }

    // Ensure function is declared on type but not yet defined
    if(fn.ctr != NULL){
      if(!contains(fn.ctr->fns, fn.name)){
        errors.pb(_err + "Function not declared on type");
        return; }
      if(fn.ctr->fns[fn.name].defined){
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
          if(enc_obj == NULL)
            fns[fn.name] = fn;
          else
            enc_obj->fns[fn.name] = fn;
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

    // Define function
    indent += 2;
    scope_table[indent] = uset<Var*>();
    fn.defined = true;
    if(enc_obj == NULL)
      enc_fn = &(fns[fn.name] = fn);
    else{
      fn.ctr = enc_obj;
      enc_fn = &(enc_obj->fns[fn.name] = fn); } }

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
      var.type = &types[toks[i]];
      if(++i >= toks.size()){
        errors.pb(_err + "Expected variable name");
        return; } }

    if(is_name(toks[i])){
      if(i + 1 >= toks.size()){
        // Variable declaration without assignment
        if(var.type != NULL){
          var.name = toks[i];
          scope_table[indent].insert(&(vars[var.name] = var));
        }else
          errors.pb(_err + "Expected operation");
        return; }

      // Send tokens to recursive function
      for(int j = 0; j < i; ++j)
        toks.erase(toks.begin());
      parse_instr(toks);

    // Syntax error
    }else{
      errors.pb(_err + "Expected variable name");
      return; } } }

// Convert an instruction into a nested function call
void Engine::parse_instr(vec<str> toks){
  const str _err = "ERR Line " + to_string(nline) + ": ";
  assert(toks.size() >= 2, "Engine.parse_instr", "Not sent enough tokens");

  // Ensure variable or function is accessible
  Var* var = NULL;
  Fn* fn = NULL;
  int i = 0;
  if(!is_name(toks[i])){
    errors.pb(_err + "Expected variable or function name");
    return; }
  if(!contains(vars, toks[i]) && !contains(fns, toks[i])){
    errors.pb(_err + "Variable or function not declared");
    return; }
  if(contains(fns, toks[i]) && !fns[toks[i]].defined){
    errors.pb(_err + "Function not defined");
    return; }
  if(contains(vars, toks[i]))
    var = &vars[toks[i]];
  else
    fn = &fns[toks[i]];
  if(++i >= toks.size()){
    errors.pb(_err + "Expected member, parenthesis, or assignment");
    return; }

  // Member
  if(toks[i] == "."){

  // Function
  }else if(toks[i] == "("){

  // Assignment
  }else if(contains(ASN_OPS, toks[i])){

  // Syntax error
  }else{
    errors.pb(_err + "Expected member, parenthesis, or assignment");
    return; }
}

// Analyze the value portion of an instruction and find its type
Type* Engine::parse_value(vec<str> toks){
  const str _err = "ERR Line " + to_string(nline) + ": ";

  //!
  return NULL; }

#endif

#include "Environment.hpp"

Env::Env() : enclosing{nullptr} {}

Env::Env(std::shared_ptr<Env> enclosing ) : enclosing{std::move(enclosing)} {}

void Env::define(const std::string& name, std::any value){
  values[name] = std::move(value);
}

std::any Env::get(const Token& name){
  auto elem = values.find(name.lexeme);
  if(elem != values.end()){
    return elem->second;
  }

  if(enclosing != nullptr){
    return enclosing->get(name);
  }

  throw RuntimeError(name, "Undefined variable: '" + name.lexeme + "'.");
}


void Env::assign(const Token& name, std::any value){
  auto elem = values.find(name.lexeme);
  if(elem != values.end()){
    elem->second = std::move(value);
    return;
  }

  if(enclosing != nullptr){
    return enclosing->assign(name, std::move(value));
  }

  throw RuntimeError(name, "Undefined variable: '" + name.lexeme + "'.");
}

#include "Function.hpp"

Function::Function(std::shared_ptr<Statement::Function> declaration,
                   std::shared_ptr<Env> closure)
    : declaration{std::move(declaration)}, closure{std::move(closure)} {};

int Function::arity() { return declaration->params.size(); }

std::any Function::call(Interpreter &interpreter,
                        std::vector<std::any> arguments) {
  auto newEnv = std::make_shared<Env>(closure);
  int size = static_cast<int>(declaration->params.size());
  for (int i = 0; i < size; i++) {
    newEnv->define(declaration->params[i].lexeme, arguments[i]);
  }

  try {
    interpreter.executeBlock(declaration->body, newEnv);
  } catch (Return returnObject) {
    return returnObject.value;
  }

  return nullptr;
}

std::string Function::toString() {
  return "<fn " + declaration->name.lexeme + ">";
}

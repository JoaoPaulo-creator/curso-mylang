#include "Interpreter.hpp"
#include "Token.hpp"
#include <chrono>
#include <memory>

Interpreter::Interpreter() {
  global->define("clock", std::shared_ptr<Callable>{});
}

int Clock::arity() { return 0; }

std::any Clock::call(Interpreter &interpreter,
                     std::vector<std::any> arguments) {
  if (arguments.size() > 0 && interpreter.global != nullptr) {
    Debug::error(1, "clock error");
  }

  auto now = std::chrono::system_clock::now();
  std::time_t time_now = std::chrono::system_clock::to_time_t(now);
  std::cout << std::ctime(&time_now) << '\n';
  return {};
}

std::string Clock::toString() { return "<fn native>"; }

std::any Interpreter::visitLiteralExpr(std::shared_ptr<Literal> expr) {
  return expr->value;
}

std::any Interpreter::visitUnaryExpr(std::shared_ptr<Unary> expr) {
  std::any right = evaluate(expr->right);

  switch (expr->oper.type) {
  case TokenType::BANG:
    return !isTruthy(right);
  case TokenType::MINUS:
    checkNumberOperand(expr->oper, right);
    return -std::any_cast<double>(right);
  default:
    return {};
  }
}

bool Interpreter::isTruthy(const std::any &object) {
  if (object.type() == typeid(nullptr))
    return false;
  if (object.type() == typeid(bool)) {
    return std::any_cast<bool>(object);
  }
  return true;
}

void Interpreter::checkNumberOperand(const Token &oper,
                                     const std::any &operand) {
  if (operand.type() == typeid(double))
    return;
  // if(operand.type() == typeid(int)) return;
  // if(operand.type() == typeid(float)) return;
  throw RuntimeError{oper, "Operand must be a number."};
}

void Interpreter::checkNumberOperands(const Token &oper, const std::any &left,
                                      const std::any &right) {
  if (left.type() == typeid(double) && right.type() == typeid(double))
    return;
  // verificar float e int
  throw RuntimeError{oper, "Operand must be a number."};
}

bool Interpreter::isEqual(const std::any &a, const std::any &b) {
  if (a.type() == typeid(nullptr) && b.type() == typeid(nullptr)) {
    return true;
  }

  if (a.type() == typeid(nullptr) || b.type() == typeid(nullptr)) {
    return false;
  }

  if (a.type() == typeid(double) && b.type() == typeid(double)) {
    return std::any_cast<double>(a) == std::any_cast<double>(b);
  }

  if (a.type() == typeid(std::string) && b.type() == typeid(std::string)) {
    return std::any_cast<std::string>(a) == std::any_cast<std::string>(b);
  }

  if (a.type() == typeid(bool) && b.type() == typeid(bool)) {
    return std::any_cast<bool>(a) == std::any_cast<bool>(b);
  }

  return false;
}

std::string Interpreter::stringify(const std::any &object) {
  if (object.type() == typeid(nullptr)) {
    return "nil";
  }

  if (object.type() == typeid(double)) {
    std::string text = std::to_string(std::any_cast<double>(object));
    if (text[text.length() - 7] == '.' && text[text.length() - 6] == '0') {
      text = text.substr(0, text.length() - 7);
    }
    return text;
  }

  if (object.type() == typeid(std::string)) {
    return std::any_cast<std::string>(object);
  }

  if (object.type() == typeid(bool)) {
    if (std::any_cast<bool>(object)) {
      std::string result{"true"};
      return result;
    } else {
      std::string result{"false"};
      return result;
    }
  }

  if (object.type() == typeid(std::shared_ptr<Function>)) {
    std::shared_ptr<Callable> function;
    function = std::any_cast<std::shared_ptr<Function>>(object);
    return function->toString();
  }

  if (object.type() == typeid(std::shared_ptr<Callable>)) {
    auto clock = std::make_shared<Clock>();
    return clock->toString();
  }

  return "stringify: cannot reconize type";
}

std::any Interpreter::visitGroupingExpr(std::shared_ptr<Grouping> expr) {
  return evaluate(expr->expression);
}

std::any Interpreter::evaluate(std::shared_ptr<Expr> expr) {
  return expr->accept(*this);
}

std::any Interpreter::visitBinaryExpr(std::shared_ptr<Binary> expr) {
  std::any left = evaluate(expr->left);
  std::any right = evaluate(expr->right);

  switch (expr->oper.type) {
  case TokenType::GREATER:
    checkNumberOperands(expr->oper, left, right);
    return std::any_cast<double>(left) > std::any_cast<double>(right);
  case TokenType::GREATER_EQUAL:
    checkNumberOperands(expr->oper, left, right);
    return std::any_cast<double>(left) >= std::any_cast<double>(right);
  case TokenType::LESS:
    checkNumberOperands(expr->oper, left, right);
    return std::any_cast<double>(left) < std::any_cast<double>(right);
  case TokenType::LESS_EQUAL:
    checkNumberOperands(expr->oper, left, right);
    return std::any_cast<double>(left) <= std::any_cast<double>(right);
  case TokenType::MINUS:
    checkNumberOperands(expr->oper, left, right);
    return std::any_cast<double>(left) - std::any_cast<double>(right);
  case TokenType::PLUS:

    if (left.type() == typeid(double) && right.type() == typeid(double)) {
      return std::any_cast<double>(left) + std::any_cast<double>(right);
    }

    if (left.type() == typeid(std::string) &&
        right.type() == typeid(std::string)) {
      return std::any_cast<std::string>(left) +
             std::any_cast<std::string>(right);
    }

    throw RuntimeError{expr->oper, "Operands not a same type"};

  case TokenType::STAR:
    checkNumberOperands(expr->oper, left, right);
    return std::any_cast<double>(left) * std::any_cast<double>(right);
  case TokenType::SLASH:
    checkNumberOperands(expr->oper, left, right);
    return std::any_cast<double>(left) / std::any_cast<double>(right);
  case TokenType::BANG_EQUAL:
    checkNumberOperands(expr->oper, left, right);
    return !isEqual(left, right);
  case TokenType::EQUAL_EQUAL:
    return isEqual(left, right);
  default:
    return {};
  }
}

void Interpreter::interpret(
    std::vector<std::shared_ptr<Statement::Stmt>> &statements) {
  try {
    for (std::shared_ptr<Statement::Stmt> &statement : statements) {
      execute(statement);
    }
  } catch (const RuntimeError &e) {
    Debug::runtimeError(e);
  }
}

void Interpreter::execute(std::shared_ptr<Statement::Stmt> statement) {
  statement->accept(*this);
}

std::any
Interpreter::visitExpressionStmt(std::shared_ptr<Statement::Expression> stmt) {
  std::any value = evaluate(stmt->expression);
  return {};
}

std::any Interpreter::visitPrintStmt(std::shared_ptr<Statement::Print> stmt) {
  std::any value = evaluate(stmt->expression);
  std::cout << stringify(value) << '\n';
  return {};
}

std::any Interpreter::visitVariableExpr(std::shared_ptr<Variable> expr) {
  std::any value = curr_env->get(expr->name);
  if (value.type() == typeid(nullptr)) {
    throw RuntimeError(expr->name, "Variable not initialized.");
  }
  return curr_env->get(expr->name);
}

std::any Interpreter::visitVarStmt(std::shared_ptr<Statement::Var> stmt) {
  std::any value = nullptr;
  if (stmt->init != nullptr) {
    value = evaluate(stmt->init);
  }
  curr_env->define(stmt->name.lexeme, std::move(value));
  return {};
}

std::any Interpreter::visitAssignExpr(std::shared_ptr<Assign> expr) {
  std::any value = evaluate(expr->value);
  global->assign(expr->name, value);
  return value;
}

void Interpreter::executeBlock(
    const std::vector<std::shared_ptr<Statement::Stmt>> &statements,
    std::shared_ptr<Env> new_env) {

  std::shared_ptr<Env> previous = curr_env;
  try {
    curr_env = new_env;
    for (const std::shared_ptr<Statement::Stmt> &statement : statements) {
      execute(statement);
    }
  } catch (...) {
    curr_env = previous;
    throw;
  }
  curr_env = previous;
}

std::any Interpreter::visitBlockStmt(std::shared_ptr<Statement::Block> stmt) {
  executeBlock(stmt->statements, std::make_shared<Env>(curr_env));
  return {};
}

std::any Interpreter::visitIfStmt(std::shared_ptr<Statement::If> stmt) {
  if (isTruthy(evaluate(stmt->condition))) {
    execute(stmt->thenBranch);
  } else if (stmt->elseBranch != nullptr) {
    execute(stmt->elseBranch);
  }
  return {};
}

std::any Interpreter::visitLogicalExpr(std::shared_ptr<Logical> expr) {
  std::any left = evaluate(expr->left);
  if (expr->oper.type == TokenType::OR) {
    if (isTruthy(left))
      return left;
  } else {
    if (!isTruthy(left))
      return left;
  }

  return evaluate(expr->right);
}

std::any Interpreter::visitWhileStmt(std::shared_ptr<Statement::While> stmt) {
  while (isTruthy(evaluate(stmt->condition))) {
    execute(stmt->body);
  }

  return {};
}

std::any Interpreter::visitCallExpr(std::shared_ptr<Call> expr) {
  std::any callee = evaluate(expr->callee);
  std::vector<std::any> arguments;
  for (std::shared_ptr<Expr> &argument : expr->arguments) {
    arguments.push_back(evaluate(argument));
  }

  std::shared_ptr<Callable> function;
  if (callee.type() == typeid(std::shared_ptr<Function>)) {
    function = std::any_cast<std::shared_ptr<Function>>(callee);
  } else if (callee.type() == typeid(std::shared_ptr<Callable>)) {
    auto clock = std::make_shared<Clock>();
    return clock->call(*this, arguments);
  } else {
    throw RuntimeError{expr->paren, "Can only call function and classes"};
  }

  if (static_cast<int>(arguments.size()) != function->arity()) {
    throw RuntimeError{expr->paren, "Expected " +
                                        std::to_string(function->arity()) +
                                        " arguments, but got " +
                                        std::to_string(arguments.size())};
  }
  return function->call(*this, std::move(arguments));
}

std::any
Interpreter::visitFunctionStmt(std::shared_ptr<Statement::Function> stmt) {
  auto function = std::make_shared<Function>(stmt, curr_env);
  curr_env->define(stmt->name.lexeme, function);
  return {};
}

std::any Interpreter::visitReturnStmt(std::shared_ptr<Statement::Return> stmt) {
  std::any value = nullptr;
  if (stmt->value != nullptr) {
    value = evaluate(stmt->value);
  }
  throw Return{value};
}

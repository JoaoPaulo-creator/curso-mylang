#pragma once

#include "Token.hpp"
#include "Visitor.hpp"
#include <memory>
#include <utility>
#include <vector>

struct Binary : Expr, public std::enable_shared_from_this<Binary> {
  std::shared_ptr<Expr> left;
  Token oper;
  std::shared_ptr<Expr> right;

  Binary(std::shared_ptr<Expr> left, Token oper, std::shared_ptr<Expr> right);
  std::any accept(ExprVisitor &visitor) override;
};

struct Grouping : Expr, public std::enable_shared_from_this<Grouping> {
  std::shared_ptr<Expr> expression;

  Grouping(std::shared_ptr<Expr> expression);
  std::any accept(ExprVisitor &visitor) override;
};

struct Literal : Expr, public std::enable_shared_from_this<Literal> {
  std::any value;

  Literal(std::any value);
  std::any accept(ExprVisitor &visitor) override;
};

struct Unary : Expr, public std::enable_shared_from_this<Unary> {
  Token oper;
  std::shared_ptr<Expr> right;

  Unary(Token oper, std::shared_ptr<Expr> right);
  std::any accept(ExprVisitor &visitor) override;
};

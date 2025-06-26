#pragma once

#include "Token.hpp"
#include "Visitor.hpp"
#include <memory>
#include <utility>
#include <vector>

struct Binary final : Expr, public std::enable_shared_from_this<Binary> {
  std::shared_ptr<Expr> left;
  Token oper;
  std::shared_ptr<Expr> right;

  Binary(std::shared_ptr<Expr> left, Token oper, std::shared_ptr<Expr> right);
  std::any accept(ExprVisitor &visitor) override;
  ~Binary() = default;
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

struct Variable final : Expr, public std::enable_shared_from_this<Variable> {
  Token name;
  Variable(Token name);
  std::any accept(ExprVisitor &visitor) override;
  ~Variable() = default;
};

struct Assign final : Expr, public std::enable_shared_from_this<Assign> {
  Token name;
  std::shared_ptr<Expr> value;

  Assign(Token name, std::shared_ptr<Expr> value);
  std::any accept(ExprVisitor &visitor);
  ~Assign() = default;
};

struct Logical final : Expr, public std::enable_shared_from_this<Logical> {
  std::shared_ptr<Expr> left;
  Token oper;
  std::shared_ptr<Expr> right;

  Logical(std::shared_ptr<Expr> left, Token oper, std::shared_ptr<Expr> right);
  std::any accept(ExprVisitor &visitor);
  ~Logical() = default;
};

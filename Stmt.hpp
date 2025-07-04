#pragma once

#include "Token.hpp"
#include "Visitor.hpp"
#include <vector>

namespace Statement {

struct Expression : Stmt, public std::enable_shared_from_this<Expression> {
  const std::shared_ptr<Expr> expression;

  Expression(std::shared_ptr<Expr> expression);
  std::any accept(StmtVisitor &visitor) override;
};

struct Print : Stmt, public std::enable_shared_from_this<Print> {
  const std::shared_ptr<Expr> expression;

  Print(std::shared_ptr<Expr> expression);
  std::any accept(StmtVisitor &visitor) override;
};

struct Var : Stmt, public std::enable_shared_from_this<Var> {
  Token name;
  std::shared_ptr<Expr> init;

  Var(Token name, std::shared_ptr<Expr> init);
  std::any accept(StmtVisitor &visitor) override;
};

struct Block : Stmt, public std::enable_shared_from_this<Block> {
  std::vector<std::shared_ptr<Stmt>> statements;

  Block(std::vector<std::shared_ptr<Stmt>> statements);
  std::any accept(StmtVisitor &visitor) override;
};

struct If : Stmt, public std::enable_shared_from_this<If> {
  std::shared_ptr<Expr> condition;
  std::shared_ptr<Stmt> thenBranch;
  std::shared_ptr<Stmt> elseBranch;

  If(std::shared_ptr<Expr> condition, std::shared_ptr<Stmt> thenBranch,
     std::shared_ptr<Stmt> elseBranch);
  std::any accept(StmtVisitor &visitor) override;
};

struct While : Stmt, public std::enable_shared_from_this<While> {
  std::shared_ptr<Expr> condition;
  std::shared_ptr<Stmt> body;

  While(std::shared_ptr<Expr> condition, std::shared_ptr<Stmt> body);
  std::any accept(StmtVisitor &visitor) override;
};

struct Function : Stmt, public std::enable_shared_from_this<Function> {
  Token name;
  std::vector<Token> params;
  std::vector<std::shared_ptr<Stmt>> body;

  Function(Token name, std::vector<Token> params,
           std::vector<std::shared_ptr<Stmt>> body);
  std::any accept(StmtVisitor &visitor) override;
};

struct Return : Stmt, public std::enable_shared_from_this<Return> {
  Token keyword;
  std::shared_ptr<Expr> value;

  Return(Token keyword, std::shared_ptr<Expr> value);
  std::any accept(StmtVisitor &visitor) override;
};

} // namespace Statement

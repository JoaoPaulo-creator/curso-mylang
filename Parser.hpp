#pragma once

#include "Debug.hpp"
#include "Expr.hpp"
#include "Scanner.hpp"
#include "Stmt.hpp"
#include <stdexcept>
#include <vector>

class Parser {
private:
  struct ParseError : public std::runtime_error {
    using std::runtime_error::runtime_error;
  };

  const std::vector<Token> tokens;
  int current = 0;

  ParseError error(const Token &, const std::string &);

  void synchronize();

  bool isAtEnd();
  bool check(const TokenType &);
  template <class... T> bool match(T... types);

  Token previous();
  Token peek();
  Token advance();
  Token consume(const TokenType &, const std::string &);

  std::shared_ptr<Expr> expression();
  std::shared_ptr<Expr> equality();
  std::shared_ptr<Expr> comparison();
  std::shared_ptr<Expr> term();
  std::shared_ptr<Expr> factor();
  std::shared_ptr<Expr> unary();
  std::shared_ptr<Expr> primary();
  std::shared_ptr<Expr> assignment();
  std::shared_ptr<Expr> logicalOr();
  std::shared_ptr<Expr> logicalAnd();

  std::shared_ptr<Statement::Stmt> statement();
  std::shared_ptr<Statement::Stmt> printStatement();
  std::shared_ptr<Statement::Stmt> expressionStatement();
  std::shared_ptr<Statement::Stmt> declaration();
  std::shared_ptr<Statement::Stmt> varDeclaration();
  std::vector<std::shared_ptr<Statement::Stmt>> block();
  std::shared_ptr<Statement::Stmt> ifStatement();

public:
  Parser(const std::vector<Token> &);
  // std::shared_ptr<Expr> parse();
  std::vector<std::shared_ptr<Statement::Stmt>> parse();
};

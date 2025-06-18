#pragma once

#include "Debug.hpp"
#include "Expr.hpp"
#include "Scanner.hpp"
#include "Token.hpp"
#include <memory>
#include <stdexcept>
#include <vector>

class Parser {
public:
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

  std::shared_ptr<Expr> expression();
  std::shared_ptr<Expr> equality();
  std::shared_ptr<Expr> comparison();
  std::shared_ptr<Expr> term();
  std::shared_ptr<Expr> factor();
  std::shared_ptr<Expr> unary();
  std::shared_ptr<Expr> primary();

public:
  Parser(std::vector<Token> &);
  std::shared_ptr<Expr> parse();
};

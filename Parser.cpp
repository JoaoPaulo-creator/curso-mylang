#include "Parser.hpp"
#include "Token.hpp"
#include "Visitor.hpp"
#include <memory>
#define assert(E)

Parser::Parser(const std::vector<Token> &tokens) : tokens(tokens) {}

std::vector<std::shared_ptr<Statement::Stmt>> Parser::parse() {
  std::vector<std::shared_ptr<Statement::Stmt>> statements;
  try {
    while (!isAtEnd()) {
      // statements.push_back(statement());
      statements.push_back(declaration());
    }
  } catch (const std::exception &e) {
    // std::cerr << "[Exception parse]: " << e.what() << '\n';
  }
  return statements;
}

std::shared_ptr<Expr> Parser::expression() {
  // return equality();
  return assignment();
}

std::shared_ptr<Expr> Parser::equality() {
  std::shared_ptr<Expr> expr = comparison();
  while (match(TokenType::BANG_EQUAL, TokenType::EQUAL_EQUAL)) {
    Token oper = previous();
    std::shared_ptr<Expr> right = comparison();
    expr = std::make_shared<Binary>(expr, oper, right);
  }
  return expr;
}

std::shared_ptr<Expr> Parser::comparison() {
  std::shared_ptr<Expr> expr = term();
  while (match(TokenType::GREATER, TokenType::GREATER_EQUAL, TokenType::LESS,
               TokenType::LESS_EQUAL)) {
    Token oper = previous();
    std::shared_ptr<Expr> right = term();
    expr = std::make_shared<Binary>(expr, oper, right);
  }
  return expr;
}

std::shared_ptr<Expr> Parser::term() {
  std::shared_ptr<Expr> expr = factor();
  while (match(TokenType::MINUS, TokenType::PLUS)) {
    Token oper = previous();
    std::shared_ptr<Expr> right = factor();
    expr = std::make_shared<Binary>(expr, oper, right);
  }
  return expr;
}

std::shared_ptr<Expr> Parser::factor() {
  std::shared_ptr<Expr> expr = unary();
  while (match(TokenType::SLASH, TokenType::STAR)) {
    Token oper = previous();
    std::shared_ptr<Expr> right = unary();
    expr = std::make_shared<Binary>(expr, oper, right);
  }
  return expr;
}

std::shared_ptr<Expr> Parser::unary() {
  while (match(TokenType::BANG, TokenType::MINUS)) {
    Token oper = previous();
    std::shared_ptr<Expr> right = unary();
    return std::make_shared<Unary>(oper, right);
  }
  return primary();
}

std::shared_ptr<Expr> Parser::primary() {
  if (match(TokenType::MY_FALSE))
    return std::make_shared<Literal>(false);
  if (match(TokenType::MY_TRUE))
    return std::make_shared<Literal>(true);
  if (match(TokenType::NIL))
    return std::make_shared<Literal>(nullptr);
  if (match(TokenType::IDENTIFIER))
    return std::make_shared<Variable>(previous());

  if (match(TokenType::NUMBER, TokenType::STRING)) {
    return std::make_shared<Literal>(previous().literal);
  }

  if (match(TokenType::LEFT_PAREN)) {
    std::shared_ptr<Expr> expr = expression();
    consume(TokenType::RIGHT_PAREN, "Expected ')' after expression.");
    return std::make_shared<Grouping>(expr);
  }

  throw error(peek(), "Expected expression.");
}

template <class... T> bool Parser::match(T... types) {
  assert((... && std::is_same_v<T, TokenType>));
  if ((... || check(types))) {
    advance();
    return true;
  }
  return false;
}

Token Parser::consume(const TokenType &token, const std::string &message) {
  if (check(token))
    return advance();
  throw error(peek(), message);
}

bool Parser::check(const TokenType &type) {
  if (isAtEnd())
    return false;
  return peek().type == type;
}

bool Parser::isAtEnd() { return peek().type == TokenType::MY_EOF; }

Token Parser::advance() {
  if (!isAtEnd())
    current++;
  return previous();
}

Token Parser::peek() { return tokens.at(current); }

Token Parser::previous() { return tokens.at(current - 1); }

Parser::ParseError Parser::error(const Token &token,
                                 const std::string &message) {
  Debug::error(token, message);
  return ParseError("");
}

void Parser::synchronize() {
  advance();
  while (!isAtEnd()) {
    if (previous().type == TokenType::SEMICOLON)
      return;
    switch (peek().type) {
    case TokenType::CLASS:
    case TokenType::FUN:
    case TokenType::VAR:
    case TokenType::FOR:
    case TokenType::IF:
    case TokenType::WHILE:
    case TokenType::PRINT:
    case TokenType::RETURN:
    default:
      return;
    }
  }
  advance();
}

std::shared_ptr<Statement::Stmt> Parser::statement() {
  if (match(TokenType::PRINT)) {
    return printStatement();
  }

  if (match(TokenType::IF)) {
    return ifStatement();
  }

  if (match(TokenType::WHILE)) {
    return whileStatement();
  }

  if (match(TokenType::FOR)) {
    return forStatement();
  }

  if (match(TokenType::LEFT_BRACE)) {
    return std::make_shared<Statement::Block>(block());
  }
  return expressionStatement();
}

std::shared_ptr<Statement::Stmt> Parser::printStatement() {
  std::shared_ptr<Expr> value = expression();
  consume(TokenType::SEMICOLON, "Expected ';' after value.");
  return std::make_shared<Statement::Print>(value);
}

std::shared_ptr<Statement::Stmt> Parser::expressionStatement() {
  std::shared_ptr<Expr> expr = expression();
  consume(TokenType::SEMICOLON, "Expected ';' after value.");
  return std::make_shared<Statement::Expression>(expr);
}

std::shared_ptr<Statement::Stmt> Parser::declaration() {
  try {
    if (match(TokenType::VAR))
      return varDeclaration();
    return statement();
  } catch (const std::exception &e) {
    synchronize();
    return nullptr;
  }
}

std::shared_ptr<Statement::Stmt> Parser::varDeclaration() {
  Token name = consume(TokenType::IDENTIFIER, "Expected variable name.");
  std::shared_ptr<Expr> init = nullptr;
  if (match(TokenType::EQUAL)) {
    init = expression();
  }
  consume(TokenType::SEMICOLON, "Expected ';' after variable declaration.");
  return std::make_shared<Statement::Var>(name, init);
}

std::shared_ptr<Expr> Parser::assignment() {
  // std::shared_ptr<Expr> expr = equality();
  std::shared_ptr<Expr> expr = logicalOr();

  if (match(TokenType::EQUAL)) {
    Token equals = previous();
    std::shared_ptr<Expr> value = assignment();
    if (Variable *e = dynamic_cast<Variable *>(expr.get())) {
      Token name = e->name;
      return std::make_shared<Assign>(std::move(name), value);
    }
    error(std::move(equals), "Invalid assignment target.");
  }
  return expr;
}

std::vector<std::shared_ptr<Statement::Stmt>> Parser::block() {
  std::vector<std::shared_ptr<Statement::Stmt>> statements;
  while (!check(TokenType::RIGHT_BRACE) && !isAtEnd()) {
    statements.push_back(declaration());
  }
  consume(TokenType::RIGHT_BRACE, "Expected '}' after block.");
  return statements;
}

std::shared_ptr<Statement::Stmt> Parser::ifStatement() {
  consume(TokenType::LEFT_PAREN, "Expected '(' after 'if' .");
  std::shared_ptr<Expr> condition = expression();
  consume(TokenType::RIGHT_PAREN, "Expected ')' after 'if' condition");
  std::shared_ptr<Statement::Stmt> thenBranch = statement();
  std::shared_ptr<Statement::Stmt> elseBranch = nullptr;

  if (match(TokenType::ELSE)) {
    elseBranch = statement();
  }

  return std::make_shared<Statement::If>(condition, thenBranch, elseBranch);
  return {};
}

std::shared_ptr<Expr> Parser::logicalOr() {
  std::shared_ptr<Expr> expr = logicalAnd();
  while (match(TokenType::OR)) {
    Token oper = previous();
    std::shared_ptr<Expr> right = logicalAnd();
    expr = std::make_shared<Logical>(expr, std::move(oper), right);
  }
  return expr;
}

std::shared_ptr<Expr> Parser::logicalAnd() {
  std::shared_ptr<Expr> expr = equality();
  while (match(TokenType::AND)) {
    Token oper = previous();
    std::shared_ptr<Expr> right = equality();
    expr = std::make_shared<Logical>(expr, std::move(oper), right);
  }

  return expr;
}

std::shared_ptr<Statement::Stmt> Parser::whileStatement() {
  consume(TokenType::LEFT_PAREN, "Expected '(' after 'while'");
  std::shared_ptr<Expr> condition = expression();
  consume(TokenType::RIGHT_PAREN, "Expected ')' after 'while' statement");
  std::shared_ptr<Statement::Stmt> body = statement();
  return std::make_shared<Statement::While>(condition, body);
}

std::shared_ptr<Statement::Stmt> Parser::forStatement() {
  consume(TokenType::LEFT_PAREN, "Expected '(' after 'for'");

  std::shared_ptr<Statement::Stmt> init;
  if (match(TokenType::SEMICOLON)) {
    init = nullptr;
  } else if (match(TokenType::VAR)) {
    init = varDeclaration();
  } else {
    init = expressionStatement();
  }

  std::shared_ptr<Expr> condition = nullptr;
  if (!check(TokenType::SEMICOLON)) {
    condition = expression();
  }

  consume(TokenType::SEMICOLON, "Expected ';' after 'for' statement");
  std::shared_ptr<Expr> increment = nullptr;

  if (!check(TokenType::RIGHT_PAREN)) {
    increment = expression();
  }

  consume(TokenType::RIGHT_PAREN, "Expected ')' after 'for' loop statement");
  std::shared_ptr<Statement::Stmt> body = statement();

  if (increment != nullptr) {
    body = std::make_shared<Statement::Block>(
        std::vector<std::shared_ptr<Statement::Stmt>>{
            body, std::make_shared<Statement::Expression>(increment)});
  }

  if (condition == nullptr) {
    condition = std::make_shared<Literal>(true);
  }

  body = std::make_shared<Statement::While>(condition, body);

  if (init != nullptr) {
    body = std::make_shared<Statement::Block>(
        std::vector<std::shared_ptr<Statement::Stmt>>{init, body});
  }

  return body;
}

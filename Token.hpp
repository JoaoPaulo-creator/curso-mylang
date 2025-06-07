#pragma once

#include "TokenType.hpp"
#include <any>
#include <iostream>
#include <sstream>
#include <string>

class Token {
public:
  TokenType type;
  std::string lexeme;
  std::any literal;
  int line;

  Token(TokenType tokenType, std::string lexeme, std::any, int);
  std::string toString();
};

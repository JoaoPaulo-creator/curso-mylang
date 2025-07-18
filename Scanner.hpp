#pragma once

#include <iostream>
#include <vector>
#include <unordered_map>
#include "Token.hpp"
#include "Debug.hpp"

class Scanner {
  private:
    int start = 0;
    int current = 0;
    int line = 1;
    std::string source;
    std::vector<Token> tokens;
    std::unordered_map<std::string, TokenType> keywords = {
      // Palavras-chave reservadas
      {"and",    TokenType::AND},
      {"class",  TokenType::CLASS},
      {"else",   TokenType::ELSE},
      {"false",  TokenType::MY_FALSE},
      {"for",    TokenType::FOR},
      {"fun",    TokenType::FUN},
      {"if",     TokenType::IF},
      {"nil",    TokenType::NIL},
      {"or",     TokenType::OR},
      {"print",  TokenType::PRINT},
      {"return", TokenType::RETURN},
      {"super",  TokenType::SUPER},
      {"this",   TokenType::THIS},
      {"true",   TokenType::MY_TRUE},
      {"var",    TokenType::VAR},
      {"while",  TokenType::WHILE}
    };

    bool isAlpha(char c);
    bool isDigit(char c);
    bool isAlphaNumeric(char c);
    bool match(char expected);
    void scanToken();
    char advance();
    void addToken(TokenType type);
    void addToken(TokenType type, std::any literal);
    char peek();
    char peekNext();
    void string();
    void number();
    void identifier();

  public:
    Scanner(const std::string&);
    bool isAtEnd();
    std::vector<Token> scanTokens();
};

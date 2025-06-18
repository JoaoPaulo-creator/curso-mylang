#include "Debug.hpp"
#include "Token.hpp"

void Debug::report(int line, const std::string &where,
                   const std::string &message) {
  hasError = true;
  std::cerr << "error - line: " << line << where << ": " << message << '\n';
}

void Debug::error(int line, const std::string &message) {
  report(line, "", message);
}

void Debug::error(Token token, const std::string &message) {
  if (token.type == TokenType::MY_EOF) {
    report(token.line, " at end", message);
  } else {
    report(token.line, " at " + token.lexeme, message);
  }
}

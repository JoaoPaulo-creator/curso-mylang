#include "Debug.hpp"

void Debug::report(int line, const std::string &where,
                   const std::string &message) {
  hasError = true;
  std::cerr << "error - line: " << line << where << ": " << message << '\n';
}

void Debug::error(int line, const std::string &message) {
  report(line, "", message);
}

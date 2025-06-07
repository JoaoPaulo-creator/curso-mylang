#pragma once
#include <iostream>

class Debug {
private:
  static void report(int, const std::string &, const std::string &);

public:
  inline static bool hasError = false;
  static void error(int line, const std::string &);
};

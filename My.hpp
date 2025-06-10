#pragma once

#include "Debug.hpp"
#include "Scanner.hpp"
#include <filesystem>
#include <fstream>
#include <iostream>
#include <sstream>
#include <vector>

class My {
private:
  static void run(const std::string &);

public:
  static void runFile(const std::string &);
  static void runPrompt();
};

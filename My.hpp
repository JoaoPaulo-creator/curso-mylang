#pragma once

#include <iostream>
#include <filesystem>
#include <vector>
#include <fstream>
#include <sstream>
#include "Scanner.hpp"
#include "Parser.hpp"
#include "Interpreter.hpp"

class My {
  private: 
    static void run(const std::string&);

  public:
    static void runFile(const std::string&);
    static void runPrompt();
};

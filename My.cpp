#include "My.hpp"
#include <vector>

namespace fs = std::filesystem;

void My::runFile(const std::string &path) {

  if (!fs::exists(path)) {
    std::cerr << "File not found.\n";
    std::exit(66);
  }

  std::ifstream file(path, std::ios::binary | std::ios::ate);
  if (!file) {
    std::cerr << "Permission error opening file.\n";
    std::exit(66);
  }

  std::streamsize size = file.tellg();
  file.seekg(0, std::ios::beg);
  std::vector<char> buffer(size);

  if (!file.read(buffer.data(), size)) {
    std::cerr << "Error reading file.\n";
    std::exit(77);
  }

  std::string content(buffer.begin(), buffer.end());
  run(content);
  if (Debug::hasError) {
    // erro de lógica
    std::exit(65);
  }
}

void My::runPrompt() {
  std::string line;
  std::cout << "my> ";
  for (;;) {
    if (!std::getline(std::cin, line) || line == "exit") {
      break;
      ;
    }
    run(line);
    std::cout << "\nmy> ";
  }

  if (Debug::hasError) {
    // erro de lógica
    std::exit(65);
  }
}

void My::run(const std::string &source) {
  Scanner scanner(source);
  std::vector<Token> tokens = scanner.scanTokens();
  for (auto &token : tokens) {
    std::cout << token.toString() << '\n';
  }
}

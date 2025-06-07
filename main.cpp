#include "My.hpp"

int main(int argc, char **argv){
  if(argc > 2){
    std::cerr << "Usage: " << argv[0] << " file.my\n";
    return EXIT_FAILURE;
  }

  if(argc == 2){
    My::runFile(argv[1]);
  }else{
    My::runPrompt();
  }

  return EXIT_SUCCESS;
}


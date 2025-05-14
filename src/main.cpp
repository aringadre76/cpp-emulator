#include "../include/console.hpp"
#include <iostream>

int main(int argc, char *argv[]) {
  if (argc < 2) {
    std::cerr << "Error: Usage: " << argv[0] << " <path_to_slug_file>"
              << std::endl;
    return 1;
  }

  std::string romPath = argv[1];
  CONSOLE console(romPath);
  console.runAll();

  return 0;
}

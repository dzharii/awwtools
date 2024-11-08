#include "internal/aww.hpp"
#include <iostream>
#include <string>
#include <vector>

int main(int argc, char** argv) {
  try {
    std::vector<std::string> cmdArgs(argv, argv + argc);
    cmdArgs.erase(cmdArgs.begin()); // remove first element
    return aww_main(cmdArgs);
  } catch (std::exception& ex) {
    std::cerr << ex.what() << "\n";
    return 1;
  } catch (...) {
    std::cerr << "Caught unknown exception.\n";
    return 1;
  }
}

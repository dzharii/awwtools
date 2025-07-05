#include "internal/aww.hpp"
#include "aww-os/aww-os.hpp"
#include <iostream>
#include <string>
#include <vector>

int main(int argc, char** argv) {
  try {
    std::vector<std::string> cmdArgs = aww::get_command_line_arguments(argc, argv);
    return aww_main(cmdArgs);
  } catch (std::exception& ex) {
    std::cerr << ex.what() << "\n";
    return 1;
  } catch (...) {
    std::cerr << "Caught unknown exception.\n";
    return 1;
  }
}

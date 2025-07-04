#include "internal/aww-tee.hpp"
#include "aww-os/aww-os.hpp"
#include <iostream>
#include <string>
#include <vector>

/*
Test everything
*/

int main(int argc, char** argv) {
  try {
    std::vector<std::string> cmdArgs = aww::get_command_line_arguments(argc, argv);
    aww::internal::aww_tee::aww_tee_io_dependencies deps;
    return aww::internal::aww_tee::aww_tee_main(cmdArgs, deps);
  } catch (std::exception& ex) {
    std::cerr << ex.what() << "\n";
    return 1;
  } catch (...) {
    std::cerr << "Caught unknown exception.\n";
    return 1;
  }
}

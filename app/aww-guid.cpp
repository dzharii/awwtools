#include <iostream>
#include <vector>
#include <string>
#include "internal/aww-guid.hpp"

int main(int argc, char **argv)
{
  try
  {
    std::vector<std::string> cmdArgs(argv, argv + argc);
    cmdArgs.erase(cmdArgs.begin()); // remove first element

    aww::internal::aww_guid::aww_guid_io_dependencies deps;
    return aww::internal::aww_guid::aww_guid_main(cmdArgs, deps);
  }
  catch (std::exception &ex)
  {
    std::cerr << ex.what() << "\n";
    return 1;
  }
  catch (...)
  {
    std::cerr << "Caught unknown exception.\n";
    return 1;
  }
}

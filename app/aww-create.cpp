#include <vector>
#include "internal/aww-create.hpp"

int main(int argc, char **argv)
{
  std::vector<std::string> cmdArgs(argv, argv + argc);
  cmdArgs.erase(cmdArgs.begin()); // remove first element

  aww::internal::aww_create::aww_create_io_dependencies deps;
  return aww::internal::aww_create::aww_create_main(cmdArgs, deps);
}


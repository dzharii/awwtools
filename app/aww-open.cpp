#include <vector>
#include <string>

#include "internal/aww-open.hpp"

int main(int argc, char **argv)
{
  std::vector<std::string> cmdArgs(argv, argv + argc);
  cmdArgs.erase(cmdArgs.begin()); // remove first element
  return aww::internal::aww_open::aww_open_main(cmdArgs);
}


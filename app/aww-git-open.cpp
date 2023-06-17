#include <vector>
#include <string>

#include "internal/aww-git-open.hpp"

int main(int argc, char **argv)
{
  std::vector<std::string> cmdArgs(argv, argv + argc);
  cmdArgs.erase(cmdArgs.begin()); // remove first element

  aww::internal::aww_git_open::aww_git_open_io_dependencies deps;
  return aww::internal::aww_git_open::aww_git_open_main(cmdArgs, deps);
}

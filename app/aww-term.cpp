#include <vector>
#include <string>
#include "internal/aww-term.hpp"

int main(int argc, char **argv)
{
  std::vector<std::string> cmdArgs(argv, argv + argc);
  cmdArgs.erase(cmdArgs.begin()); // remove first element
  return aww::internal::aww_term::aww_term_main(cmdArgs);
}

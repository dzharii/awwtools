#include <vector>
#include <string>
#include "internal/aww-tag.hpp"

int main(int argc, char **argv)
{
  std::vector<std::string> cmdArgs(argv, argv + argc);
  cmdArgs.erase(cmdArgs.begin()); // remove first element
  return aww::internal::aww_tag::aww_tag_main(cmdArgs);
}

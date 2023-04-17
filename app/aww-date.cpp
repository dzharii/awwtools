#include <vector>
#include <string>
#include "internal/aww-date.hpp"

int main(int argc, char **argv)
{
  std::vector<std::string> cmdArgs(argv, argv + argc);
  cmdArgs.erase(cmdArgs.begin()); // remove first element
  return aww::internal::aww_date::aww_date_main(cmdArgs);
}

#include <vector>
#include <string>
#include "internal/aww-toast.hpp"

int main(int argc, char **argv)
{
  std::vector<std::string> cmdArgs(argv, argv + argc);
  cmdArgs.erase(cmdArgs.begin()); // remove first element
  return aww::internal::aww_toast::aww_toast_main(cmdArgs);
}

#include <filesystem>
#include "aww-common.hpp"
namespace aww::os::actions
{
  aww::result_t launchFile(const std::string &path)
  {
    return std::make_tuple(false, "launchFile is not implemented for this Operating System :(");
  }
}

namespace aww::fs
{
  std::filesystem::path getCurrentExecutablePath(void)
  {
    return std::filesystem::path();
  }
}

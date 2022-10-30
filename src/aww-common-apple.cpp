#include <filesystem>
#include <cstdlib>
#include "aww-common.hpp"

#ifdef APPLE
  #include <mach-o/dyld.h>
#endif

namespace aww::os::actions
{

  aww::result_t launchFile(const std::string &path)
  {
    // check path is null
    if (path.empty())
    {
      return std::make_tuple(false, "Argument path is empty");
    }
    std::string command = "open " + path;
    int result = std::system(command.c_str());
    if (result == 0)
    {
      return std::make_tuple(true, "");
    }
    return std::make_tuple(false, "open failed");
  }
}

namespace aww::fs
{
  #ifdef APPLE
    std::filesystem::path getCurrentExecutablePath(void)
    {
      char result[PATH_MAX];
      uint32_t size = sizeof(result);
      if (_NSGetExecutablePath(result, &size) == 0)
      {
        return std::filesystem::path(result);
      }
      return std::filesystem::path();
    }
  #endif
}

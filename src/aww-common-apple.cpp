#include <filesystem>
#include <cstdlib>
#include "aww-common.hpp"
#include <mach-o/dyld.h>


namespace aww::os {
  bool canExecute(const std::filesystem::path &path)
  {
    const bool isExecutable = std::filesystem::exists(path)
                              && access(path.c_str(), X_OK) == 0;
    return isExecutable;
  }
}

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

  aww::result_t showNotification(
    const std::string &title,
    const std::string &message)
  {
    // check title is null
    if (title.empty())
    {
      return std::make_tuple(false, "Argument title is empty");
    }
    // check message is null
    if (message.empty())
    {
      return std::make_tuple(false, "Argument message is empty");
    }
    std::string command = "osascript -e 'display notification \"" + message + "\" with title \"" + title + "\"'";
    int result = std::system(command.c_str());
    if (result != 0)
    {
      return std::make_tuple(false, "osascript failed");
    }
    return std::make_tuple(true, "");
  }
}

namespace aww::fs
{
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
}

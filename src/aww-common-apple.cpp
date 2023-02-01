#include <filesystem>
#include <fstream>
#include <cstdlib>
#include "aww-common.hpp"
#include <mach-o/dyld.h>
#include <unistd.h>
#include <CoreFoundation/CoreFoundation.h>

namespace fs = std::filesystem;

namespace aww::os {

  std::string escapeCommandLineArgs(const std::string args) {
    std::string escapedArgs(args.size(), ' ');
    for (char c : args) {
      if (c == '"') {
        escapedArgs += "\\\"";
      } else {
        escapedArgs += c;
      }
    }
    return escapedArgs;
  }

  bool canExecute(const std::filesystem::path &path)
  {
    if (!std::filesystem::exists(path)) {
      return false;
    }
    if (std::filesystem::is_directory(path)) {
      return false;
    }

    std::filesystem::perms perms = std::filesystem::status(path).permissions();

    bool anyExecute = (perms & std::filesystem::perms::owner_exec) != std::filesystem::perms::none ||
                      (perms & std::filesystem::perms::group_exec) != std::filesystem::perms::none ||
                      (perms & std::filesystem::perms::others_exec) != std::filesystem::perms::none;

    return anyExecute;
  }

  std::vector<std::string> getCommandLineArgs(void)
  {
    pid_t pid = getpid();
    std::vector<std::string> args;
    std::string filename = "/proc/" + std::to_string(pid) + "/cmdline";
    std::ifstream file(filename);
    if (file.is_open())
    {
        std::string arg;
        while (std::getline(file, arg, '\0'))
        {
            args.push_back(arg);
        }
    }
    return args;
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

namespace aww::util
{
  aww::result_t getGuid(std::string &out) {
    CFUUIDRef uuid = CFUUIDCreate(NULL);
    CFStringRef str = CFUUIDCreateString(NULL, uuid);

    char buf[37];
    CFStringGetCString(str, buf, sizeof(buf), kCFStringEncodingUTF8);

    out = buf;

    CFRelease(str);
    CFRelease(uuid);

    return std::make_tuple(true, "");
  }
}

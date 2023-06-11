#include <filesystem>
#include <fstream>
#include <cstdlib>
#include "aww-common.hpp"
#include <unistd.h>
#include <limits.h>

namespace aww::os {

  bool can_execute(const std::filesystem::path &path)
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

  std::vector<std::string> get_command_line_args(void)
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
  aww::Result launch_file(const std::string &path)
  {
    // check path is null
    if (path.empty())
    {
      return aww::Result::fail("Argument path is empty");
    }
    std::string command = "xdg-open " + path;
    int result = std::system(command.c_str());
    if (result == 0)
    {
      return aww::Result::ok();
    }
    return aww::Result::fail("xdg-open failed");
  }

  aww::Result show_notification(
    const std::string &title,
    const std::string &message)
  {
    // check title is null
    if (title.empty())
    {
      return aww::Result::fail("Argument title is empty");
    }
    // check message is null
    if (message.empty())
    {
      return aww::Result::fail("Argument message is empty");
    }
    std::string command = "notify-send \"" + title + "\" \"" + message + "\"";
    int result = std::system(command.c_str());
    if (result != 0)
    {
      return aww::Result::fail("notify-send failed");
    }
    return aww::Result::ok();
  }
}

namespace aww::fs
{
    std::filesystem::path get_current_executable_path(void)
    {
      char result[PATH_MAX];
      ssize_t count = readlink("/proc/self/exe", result, PATH_MAX);
      return std::filesystem::path(std::string(result, (count > 0) ? count : 0));
    }
}

namespace aww::util
{
  // Implements GUID generations for linux using /proc/sys/kernel/random/uuid
  aww::Result get_guid(std::string &out) {
    const static std::string uuidFile = "/proc/sys/kernel/random/uuid";
    std::ifstream file(uuidFile);
    if (file.is_open())
    {
        std::getline(file, out);
        return aww::Result::ok();
    }
    return aww::Result::fail("Failed to open " + uuidFile);
  }
}

#include <iostream>
#include <string>
#include <vector>
#include <filesystem>
#include <map>
#include <array>

#include "aww-common.hpp"

namespace aww::internal::aww_open
{
  namespace fs = std::filesystem;

  int aww_open_main(const std::vector<std::string> &cmdArgs)
  {
    bool useDefaultInput = cmdArgs.size() == 0;

    if (cmdArgs.size() > 1)
    {
      std::cout << "Too many arguments provided. Expected 1: file to open"
                << "\n";
      return 1;
    }

    const std::string DEFAULT_OPEN_PATH = "."; // Current directory
    std::string fileToOpen = useDefaultInput ? DEFAULT_OPEN_PATH : cmdArgs[0];

    fileToOpen = aww::fs::normalize_file_path(fileToOpen);

    aww::Result launchFileRes = aww::os::actions::launch_file(fileToOpen);
    if (launchFileRes.is_failed())
    {
      std::cout << "Failed to launch file " << launchFileRes.error() << "\n";
      aww::os::actions::show_notification("aww open", "Failed to open file: " + fileToOpen);
      return 1;
    }
    std::cout << "Launched file"
              << "\n";
    aww::os::actions::show_notification("aww open", "The file was opened");
    return 0;
  }
}

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

  int aww_open_main(int argc, char **argv)
  {
    bool useDefaultInput = argc < 2;

    if (argc > 2)
    {
      std::cout << "Too many arguments provided. Expected 1: file to open"
                << "\n";
      return 1;
    }

    std::string fileToOpen = useDefaultInput ? "." : argv[1];

    fileToOpen = aww::fs::normalizeFilePath(fileToOpen);

    aww::Result launchFileRes = aww::os::actions::launchFile(fileToOpen);
    if (launchFileRes.isFailed())
    {
      std::cout << "Failed to launch file " << launchFileRes.error() << "\n";
      aww::os::actions::showNotification("aww open", "Failed to open file: " + fileToOpen);
      return 1;
    }
    std::cout << "Launched file"
              << "\n";
    aww::os::actions::showNotification("aww open", "The file was opened");
    return 0;
  }
}

#include <array>
#include <filesystem>
#include <iostream>
#include <map>
#include <string>
#include <vector>

#include "aww-common.hpp"
#include "aww-spdlog-configuration.hpp"
#include "internal/aww-open.hpp"

namespace aww::internal::aww_open {
namespace fs = std::filesystem;

int aww_open_main(const std::vector<std::string>& cmdArgs, aww_open_io_dependencies_interface& deps) {
  init_default_spdlog_configuration("aww-open");
  bool useDefaultInput = cmdArgs.size() == 0;

  if (cmdArgs.size() > 1) {
    std::cout << "Too many arguments provided. Expected 1: file to open"
              << "\n";
    return 1;
  }

  const std::string DEFAULT_OPEN_PATH = "."; // Current directory
  std::string fileToOpen = useDefaultInput ? DEFAULT_OPEN_PATH : cmdArgs[0];

  fileToOpen = aww::fs::normalize_file_path(fileToOpen);

  if (aww::Result res = deps.launch_file(fileToOpen); res.is_failed()) {
    std::cout << "Failed to launch file " << res.error() << "\n";
    deps.show_notification("aww open", "Failed to open file: " + fileToOpen);
    return 1;
  }
  std::cout << "Launched file"
            << "\n";
  deps.show_notification("aww open", "The file was opened");
  return 0;
}
} // namespace aww::internal::aww_open

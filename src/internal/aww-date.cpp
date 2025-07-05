#include <iostream>
#include <regex>
#include <string>

#include "aww-spdlog-configuration.hpp"
#include "fmt/core.h"
#include "internal/aww-date.hpp"
#include "spdlog/spdlog.h"

namespace aww::internal::aww_date {

int aww_date_main(const std::vector<std::string>& cmdArgs, aww_date_io_dependencies_interface& deps) {
  init_default_spdlog_configuration("aww-date");
  auto mutableCmdArgs = cmdArgs;

  bool noLogging = aww::erase_flag_from_args(mutableCmdArgs, aww::constants::CMD_FLAG_NO_LOGGING);
  bool noNotifications = aww::erase_flag_from_args(mutableCmdArgs, aww::constants::CMD_FLAG_NO_NOTIFICATIONS);

  // Configure spdlog based on the flag
  if (noLogging) {
    spdlog::set_level(spdlog::level::off);
  } else {
    spdlog::set_level(spdlog::level::info); // Set desired log level
  }

  std::string currentDate = deps.get_date_yyyymmdd();

  std::string fileName = aww::string::join(mutableCmdArgs, "-");
  std::regex replaceFilenameUnsafeChars("[^\\._a-zA-Z0-9-]");
  std::string safeFileName = std::regex_replace(fileName, replaceFilenameUnsafeChars, "-");

  std::string result = currentDate;

  if (!safeFileName.empty()) {
    result = result + "-" + safeFileName;
  }
  if (deps.clipboard_set_text(result)) {
    spdlog::info("Copied to clipboard: {}", result);

    if (!noNotifications) {
      deps.show_notification("aww date", "The date has been copied to the clipboard");
    }
  } else {
    spdlog::error("Failed to copy to clipboard: {}", result);
    if (!noNotifications) {
      deps.show_notification("aww date", "Failed to copy the date to the clipboard");
    }
    return 1;
  }

  std::cout << result << "\n";
  return 0;
}

} // namespace aww::internal::aww_date

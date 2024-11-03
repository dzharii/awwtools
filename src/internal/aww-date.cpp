#include <iostream>
#include <regex>
#include <string>

#include "fmt/core.h"
#include "internal/aww-date.hpp"
#include "spdlog/spdlog.h"

namespace aww::internal::aww_date {

int aww_date_main(const std::vector<std::string>& cmdArgs,
                  aww_date_io_dependencies_interface& deps) {

  auto mutableCmdArgs = cmdArgs;

  bool noLogging = aww::erase_flag_from_args(mutableCmdArgs, aww::constants::CMD_FLAG_NO_LOGGING);
  bool noNotifications =
      aww::erase_flag_from_args(mutableCmdArgs, aww::constants::CMD_FLAG_NO_NOTIFICATIONS);

  // Configure spdlog based on the flag
  if (noLogging) {
    spdlog::set_level(spdlog::level::off);
  } else {
    spdlog::set_level(spdlog::level::info); // Set desired log level
  }

  if (mutableCmdArgs.size() == 0) {
    spdlog::warn("No arguments provided");
    return 1;
  }

  std::string currentDate = deps.get_date_yyyymmdd(aww::call_tag("1rfpeonkhns"));

  std::string fileName = aww::string::join(mutableCmdArgs, "-");
  std::regex replaceFilenameUnsafeChars("[^\\._a-zA-Z0-9-]");
  std::string safeFileName = std::regex_replace(fileName, replaceFilenameUnsafeChars, "-");

  std::string result = currentDate;

  if (!safeFileName.empty()) {
    result = result + "-" + safeFileName;
  }
  if (deps.clipboard_set_text(result, aww::call_tag("t7svmrrhai0"))) {
    spdlog::info("Copied to clipboard: {}", result);

    if (!noNotifications) {
      deps.show_notification("aww date", "The date has been copied to the clipboard",
                             aww::call_tag("tssis4p5ta2"));
    }
  } else {
    spdlog::error("Failed to copy to clipboard: {}", result);
    if (!noNotifications) {
      deps.show_notification("aww date", "Failed to copy the date to the clipboard",
                             aww::call_tag("730v5jc2d3o"));
    }
    return 1;
  }

  std::cout << result << "\n";
  return 0;
}

} // namespace aww::internal::aww_date

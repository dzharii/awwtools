#include <iostream>
#include <string>

#include "aww-spdlog-configuration.hpp"
#include "aww-special-string/aww-special-string.hpp"
#include "fmt/core.h"
#include "internal/aww-date.hpp"
#include "spdlog/spdlog.h"

namespace aww::internal::aww_date {

int aww_date_main(const std::vector<std::string>& cmdArgs, aww_date_io_dependencies_interface& deps) {
  init_default_spdlog_configuration("aww-date");
  auto mutable_cmd_args = cmdArgs;

  bool no_logging_flag = aww::erase_flag_from_args(mutable_cmd_args, aww::constants::CMD_FLAG_NO_LOGGING);
  bool no_notifications_flag = aww::erase_flag_from_args(mutable_cmd_args, aww::constants::CMD_FLAG_NO_NOTIFICATIONS);

  // Configure spdlog based on the flag
  if (no_logging_flag) {
    spdlog::set_level(spdlog::level::off);
  } else {
    spdlog::set_level(spdlog::level::info); // Set desired log level
  }

  std::string current_date = deps.get_date_yyyymmdd();

  std::string filename = aww::string::join(mutable_cmd_args, "-");
  std::string safe_filename = aww::safe_filename_from_string(filename, '-');

  std::string result = current_date;

  if (!safe_filename.empty()) {
    result = result + "-" + safe_filename;
  }
  if (deps.clipboard_set_text(result)) {
    spdlog::info("Copied to clipboard: {}", result);

    if (!no_notifications_flag) {
      deps.show_notification("aww date", "The date has been copied to the clipboard");
    }
  } else {
    spdlog::error("Failed to copy to clipboard: {}", result);
    if (!no_notifications_flag) {
      deps.show_notification("aww date", "Failed to copy the date to the clipboard");
    }
    return 1;
  }

  std::cout << result << "\n";
  return 0;
}

} // namespace aww::internal::aww_date

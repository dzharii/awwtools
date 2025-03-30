#ifndef AWW_SPDLOG_CONFIGURATION_HPP
#define AWW_SPDLOG_CONFIGURATION_HPP

#include <filesystem>
#include <stdexcept>
#include <string>

#include "aww-os/aww-os.hpp"
#include "spdlog/sinks/basic_file_sink.h"
#include "spdlog/spdlog.h"

#include "fmt/core.h"

namespace fs = std::filesystem;

/**
 * @brief Sets up the default spdlog logger using a validated log file name. (aww tag #woar96msh50)
 *
 * @param log_name Alphanumeric, dash (-), or underscore (_) name for the log file.
 * @throws std::invalid_argument If empty or contains invalid characters.
 */
inline void init_default_spdlog_configuration(std::string log_name) {
  if (log_name.empty()) {
    throw std::invalid_argument("#u7vgywi3wm7 the #argument=`log_name` cannot be empty");
  }

  constexpr size_t max_log_file_name_size = 100;
  std::string trimmed_log_file_name =
      log_name.length() > max_log_file_name_size ? log_name.substr(0, max_log_file_name_size) : log_name;
  for (char c : log_name) {
    if (!std::isalnum(static_cast<unsigned char>(c)) && c != '-' && c != '_') {
      throw std::invalid_argument(
          fmt::format("#g6wz13fp4xx the #argument=`log_name` must contain only letters, digits, dashes (-), or "
                      "underscores (_). The actual value was #log_name=`{}`",
                      trimmed_log_file_name));
    }
  }

  auto exe_path = aww::get_current_executable_path();
  auto exe_folder = exe_path.parent_path();
  fs::path logs_folder = exe_folder / "logs";
  if (!fs::exists(logs_folder)) {
    fs::create_directory(logs_folder);
  }
  fs::path log_file = logs_folder / (trimmed_log_file_name + ".log");

  std::string app_logger_name = "app_logger_" + trimmed_log_file_name;

  // Only create logger if it doesn't already exist
  auto existing_logger = spdlog::get(app_logger_name);
  if (!existing_logger) {
    auto logger = spdlog::basic_logger_mt(app_logger_name, log_file.string());
    spdlog::set_default_logger(logger);
    spdlog::set_level(spdlog::level::info);
    spdlog::flush_every(std::chrono::seconds(1));
  } else {
    spdlog::set_default_logger(existing_logger);
  }
}

#endif // AWW_SPDLOG_CONFIGURATION_HPP

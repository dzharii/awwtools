#include "internal/aww-tee.hpp"
#include "aww-os/aww-os.hpp"
#include "fmt/core.h"
#include "spdlog/sinks/basic_file_sink.h"
#include "spdlog/spdlog.h"
#include "webview/webview.h"
#include <filesystem>
#include <iostream>
#include <string>
#include <vector>

namespace aww::internal::aww_tee {
namespace fs = std::filesystem;

int aww_tee_main([[maybe_unused]] const std::vector<std::string>& cmd_args,
                 [[maybe_unused]] aww_tee_io_dependencies_interface& deps) {
  spdlog::info("aww tee");

  try {

    auto exe_path = aww::get_current_executable_path();
    auto exe_folder = exe_path.parent_path();
    fs::path logs_folder = exe_folder / "logs";
    if (!fs::exists(logs_folder)) {
      fs::create_directory(logs_folder);
    }
    fs::path log_file = logs_folder / "app.log";
    auto logger = spdlog::basic_logger_mt("app_logger", log_file.string());
    spdlog::set_default_logger(logger);
    spdlog::set_level(spdlog::level::info);
    spdlog::flush_every(std::chrono::seconds(1));
    spdlog::info("Application starting.");

    // webview
    webview::webview w(false, nullptr);
    w.set_title("Basic Example");
    w.set_size(480, 320, WEBVIEW_HINT_NONE);
    w.set_html("Thanks for using webview!");
    w.eval("2 + 2");
    w.run();
    spdlog::info("Application exiting normally.");
  } catch (const webview::exception& e) {
    spdlog::critical("Unhandled exception in main: {}", e.what());
    return 1;
  }

  return 0;
}

} // namespace aww::internal::aww_tee

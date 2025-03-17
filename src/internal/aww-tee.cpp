#include "internal/aww-tee.hpp"
#include "aww-os/aww-os.hpp"
#include "fmt/core.h"
#include "spdlog/sinks/basic_file_sink.h"
#include "spdlog/spdlog.h"
#include "webview/webview.h"
#include <filesystem>
#include <iostream>
#include <string>
#include <thread>
#include <vector>

#ifdef _WIN32
#include <io.h>
#define isatty _isatty
#define fileno _fileno
#else
#include <unistd.h>
#endif

namespace aww::internal::aww_tee {
namespace fs = std::filesystem;

// Helper function to escape a string for insertion into a JavaScript string literal.
std::string escape_js(const std::string& input) {
  std::string output = "\"";
  for (char c : input) {
    switch (c) {
    case '\\':
      output += "\\\\";
      break;
    case '\"':
      output += "\\\"";
      break;
    case '\'':
      output += "\\\'";
      break;
    case '\n':
      output += "\\n";
      break;
    case '\r':
      output += "\\r";
      break;
    default:
      output.push_back(c);
      break;
    }
  }
  output += "\"";
  return output;
}

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

    // Create the webview with an initial HTML that holds an empty <ul>
    webview::webview w(false, nullptr);
    w.set_title("Basic Example");
    w.set_size(480, 320, WEBVIEW_HINT_NONE);
    std::string html = "<html><head><meta charset=\"utf-8\"></head>"
                       "<body>Hello<ul id=\"log\"></ul></body></html>";
    w.set_html(html);

    // Check if standard input is redirected (i.e. not a terminal)
    bool has_redirected_input = !isatty(fileno(stdin));
    spdlog::info("#has_redirected_input = {}", has_redirected_input);

    std::thread input_thread;
    if (has_redirected_input) {
      input_thread = std::thread([&w]() {
        std::string line;
        // Read input line by line until EOF
        while (std::getline(std::cin, line)) {
          spdlog::info("Received input: {}", line);
          // Build JS code to append a new <li> element to the <ul>
          std::string js = fmt::format("document.body.textContent += {};", escape_js(line));

          try {
            w.eval(js);
          } catch (const std::exception& e) {
            spdlog::error("Error evaluating JS: {}", e.what());
          }
        }
      });
    }

    // Run the webview loop (this call blocks until the window is closed)
    w.run();
    spdlog::info("Application exiting normally.");

    if (input_thread.joinable()) {
      input_thread.join();
    }
  } catch (const webview::exception& e) {
    spdlog::critical("Unhandled exception in main: {}", e.what());
    return 1;
  }

  return 0;
}

} // namespace aww::internal::aww_tee

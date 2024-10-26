#include <iostream>
#include <regex>
#include <string>

#include "fmt/core.h"
#include "internal/aww-date.hpp"
#include "spdlog/spdlog.h"

namespace aww::internal::aww_date {

int aww_date_main(const std::vector<std::string>& cmdArgs,
                  aww_date_io_dependencies_interface& deps) {
  spdlog::info("Hello spdlog");
  fmt::print("Hello, world from fmt PLEASE REMOVE THIS \b!\n");

  std::string currentDate = deps.get_date_yyyymmdd(aww::call_tag("1rfpeonkhns"));

  std::string fileName = aww::string::join(cmdArgs, "-");
  std::regex replaceFilenameUnsafeChars("[^\\._a-zA-Z0-9-]");
  std::string safeFileName = std::regex_replace(fileName, replaceFilenameUnsafeChars, "-");

  std::string result = currentDate;

  if (!safeFileName.empty()) {
    result = result + "-" + safeFileName;
  }
  if (deps.clipboard_set_text(result, aww::call_tag("t7svmrrhai0"))) {
    std::cout << "Copied to clipboard: " << result << "\n";
    deps.show_notification("aww date", "The date has been copied to the clipboard",
                           aww::call_tag("tssis4p5ta2"));
  } else {
    std::cout << "Failed to copy to clipboard: " << result << "\n";
    deps.show_notification("aww date", "Failed to copy the date to the clipboard",
                           aww::call_tag("730v5jc2d3o"));
    return 1;
  }

  std::cout << "Result:"
            << "\n"
            << result << "\n";

  return 0;
}

} // namespace aww::internal::aww_date

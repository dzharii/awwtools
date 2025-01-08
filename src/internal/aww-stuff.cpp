#include <iostream>
#include <string>

#include "fmt/core.h"
#include "spdlog/spdlog.h"

#include "internal/aww-stuff.hpp"

namespace aww::internal::aww_stuff {

int aww_stuff_main([[maybe_unused]] const std::vector<std::string>& cmdArgs,
                   aww_stuff_io_dependencies_interface& deps) {
  spdlog::info("aww stuff is a test tool to to do all kinds of stuff");

  constexpr auto save_clipboard_to_file = "save-clipboard-to-file";

  if (cmdArgs.size() == 1 && cmdArgs[0] == save_clipboard_to_file) {
    // TODO
    return 0;
  }

  if (cmdArgs.size() == 0) {
  }
  deps.show_notification("aww stuff", "It works", aww::call_tag("pvdx2hbfwaz"));
  return 0;
}

} // namespace aww::internal::aww_stuff

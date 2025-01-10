#include <iostream>
#include <string>

#include "fmt/core.h"
#include "spdlog/spdlog.h"

#include "aww-common.hpp"
#include "clip.h"

#include "internal/aww-stuff.hpp"

namespace aww::internal::aww_stuff {

int aww_stuff_main([[maybe_unused]] const std::vector<std::string>& cmdArgs,
                   [[maybe_unused]] aww_stuff_io_dependencies_interface& deps) {
  spdlog::info("aww stuff is a test tool to to do all kinds of stuff");

  constexpr auto saveClipboardToFile = "save-clipboard-to-file";
  constexpr auto setClipboardFromFile = "set-clipboard-from-file";

  if (cmdArgs.size() >= 1 && cmdArgs[0] == saveClipboardToFile) {
    spdlog::info("Saving clipboard to file");
    std::string guid;
    aww::Result awwresult = aww::util::get_guid(guid);
    if (awwresult.is_failed()) {
      spdlog::error("Failed to get guid");
      return 1;
    }

    std::string clipboardText{};
    if (!clip::get_text(clipboardText)) {
      spdlog::error("Failed to get clipboard text");
      return 1;
    }

    fmt::print("Clipboard text: {}\n", clipboardText);

    // display formatted character hex codes
    for (const auto& c : clipboardText) {
      fmt::print("{:02x} ", static_cast<unsigned char>(c));
    }

    const std::string fileName = fmt::format("clipboard-{}.txt", guid);
    spdlog::info("Writing clipboard to file: '{}'", fileName);

    const std::vector<std::string> fileContent{clipboardText};
    auto writeResult = aww::fs::write_lines(fileName, fileContent);
    if (writeResult.is_failed()) {
      spdlog::error("Failed to write clipboard to file");
      return 1;
    }
    spdlog::info("Clipboard saved to file");
    return 0;
  } else if (cmdArgs.size() >= 2 && cmdArgs[0] == setClipboardFromFile) {
    spdlog::info("Setting clipboard from file");
    const std::string fileName = cmdArgs[1];
    spdlog::info("Reading clipboard from file: '{}'", fileName);

    std::vector<std::string> fileContent;
    auto readResult = aww::fs::read_lines(fileName, fileContent);
    if (readResult.is_failed()) {
      spdlog::error("Failed to read clipboard from file");
      return 1;
    }

    std::string clipboardText = aww::string::join(fileContent, "\n");
    spdlog::info("Clipboard text: '{}'", clipboardText);

    if (!clip::set_text(clipboardText)) {
      spdlog::error("Failed to set clipboard text");
      return 1;
    }

    spdlog::info("Clipboard set from file");
    return 0;
  } else {

    fmt::print("Usage: aww-stuff <command>\n");
    fmt::print("Commands:\n");
    fmt::print("  {} - Save the clipboard to a file\n", saveClipboardToFile);
    fmt::print("  {} file_name - Set the clipboard from a file\n", setClipboardFromFile);
  }
  return -1;
}

} // namespace aww::internal::aww_stuff

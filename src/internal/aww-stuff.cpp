#include <iostream>
#include <string>

#include "fmt/core.h"
#include "spdlog/spdlog.h"

#include "aww-common.hpp"
#include "clip.h"

#include "internal/aww-stuff.hpp"

namespace aww::internal::aww_stuff {

/**
 * Save the current clipboard text to a file.
 * @return 0 on success, 1 on failure.
 */
int saveClipboardToFile() {
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
}

/**
 * Set the clipboard text from a file.
 * @param fileName The name of the file to read from.
 * @return 0 on success, 1 on failure.
 */
int setClipboardFromFile(const std::string& fileName) {
  spdlog::info("Setting clipboard from file");
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
}

/**
 * Save the command line arguments to a file.
 * @param cmdArgs Command line arguments.
 * @return 0 on success, 1 on failure.
 */
int saveCommandLineArgsToFile(const std::vector<std::string>& cmdArgs) {
  spdlog::info("Saving command line arguments to file");
  std::string guid;
  aww::Result awwresult = aww::util::get_guid(guid);
  if (awwresult.is_failed()) {
    spdlog::error("Failed to get guid");
    return 1;
  }

  std::vector<std::string> fileContent;
  for (const auto& arg : cmdArgs) {
    fileContent.push_back(fmt::format("\"{}\"", arg));
    std::string hexRepresentation;
    for (const auto& c : arg) {
      hexRepresentation += fmt::format("{:02x} ", static_cast<unsigned char>(c));
    }
    fileContent.push_back(fmt::format("Bytes: {}", hexRepresentation));
  }

  const std::string fileName = fmt::format("cmd-args-{}.txt", guid);
  spdlog::info("Writing command line arguments to file: '{}'", fileName);

  auto writeResult = aww::fs::write_lines(fileName, fileContent);
  if (writeResult.is_failed()) {
    spdlog::error("Failed to write command line arguments to file");
    return 1;
  }
  spdlog::info("Command line arguments saved to file");
  return 0;
}

int aww_stuff_main([[maybe_unused]] const std::vector<std::string>& cmdArgs,
                   [[maybe_unused]] aww_stuff_io_dependencies_interface& deps) {
  spdlog::info("aww stuff is a test tool to to do all kinds of stuff");

  constexpr auto saveClipboardToFileCmd = "save-clipboard-to-file";
  constexpr auto setClipboardFromFileCmd = "set-clipboard-from-file";
  constexpr auto saveCommandLineArgsToFileCmd = "save-command-line-args-to-file";

  if (cmdArgs.size() >= 1 && cmdArgs[0] == saveClipboardToFileCmd) {
    return saveClipboardToFile();
  } else if (cmdArgs.size() >= 2 && cmdArgs[0] == setClipboardFromFileCmd) {
    return setClipboardFromFile(cmdArgs[1]);
  } else if (cmdArgs.size() >= 1 && cmdArgs[0] == saveCommandLineArgsToFileCmd) {
    return saveCommandLineArgsToFile(cmdArgs);
  } else {
    fmt::print("Usage: aww-stuff <command>\n");
    fmt::print("Commands:\n");
    fmt::print("  {} - Save the clipboard to a file\n", saveClipboardToFileCmd);
    fmt::print("  {} file_name - Set the clipboard from a file\n", setClipboardFromFileCmd);
    fmt::print("  {} - Save command line arguments to a file\n", saveCommandLineArgsToFileCmd);
  }
  return -1;
}

} // namespace aww::internal::aww_stuff

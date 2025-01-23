#include <iostream>
#include <string>

#include "fmt/core.h"
#include "spdlog/spdlog.h"

#include "aww-common.hpp"
#include "clip.h"

#include <fstream>
#include <functional>
#include <iostream>
#include <string>
#include <vector>

#include "aww-result/aww-result.hpp"
#include "fmt/core.h"
#include "spdlog/spdlog.h"

#include "internal/aww-stuff.hpp"

namespace aww::internal::aww_stuff {

/** cat command, mainly to test aww::result
 *
 */

// Utility function to check if a file is text
aww::result<bool> is_text_file(const std::string& file_path) {
  std::ifstream file(file_path, std::ios::binary);
  if (!file.is_open()) {
    return aww::result<bool>::err(fmt::format("File does not exist or cannot be opened. File path is '{}'", file_path));
  }

  char buffer[512];
  file.read(buffer, sizeof(buffer));
  auto bytes_read = file.gcount();

  for (std::streamsize i = 0; i < bytes_read; ++i) {
    if (buffer[i] == '\0') {
      return aww::result<bool>::err("File appears to be binary");
    }
  }

  return aww::result<bool>::ok(true);
}

// Function to read file content in buffered chunks
using on_data_callback = std::function<void(const char*, size_t)>;
aww::result<bool> read_file_buffered(const std::string& file_path, on_data_callback& on_data) {
  std::ifstream file(file_path);
  if (!file.is_open()) {
    return aww::result<bool>::err(fmt::format("Failed to open file. File path is '{}'", file_path));
  }

  constexpr size_t buffer_size = 1024;
  char buffer[buffer_size];

  while (file.read(buffer, buffer_size) || file.gcount() > 0) {
    on_data(buffer, file.gcount());
    // std::cout.write(buffer, file.gcount());
  }

  if (!file.eof()) {
    return aww::result<bool>::err("Error occurred while reading the file");
  }

  return aww::result<bool>::ok(true);
}

// Command implementation for "cat"
int cat_command(const std::string& file_path) {
  spdlog::info("Executing 'cat' command for file: {}", file_path);

  // Step 1: Validate the file
  auto text_check = is_text_file(file_path);
  if (text_check.is_err()) {
    spdlog::error("Validation failed: {}", text_check.error().error_message());
    return -1;
  }

  // Step 2: Read and display the file content
  on_data_callback on_data_print_to_stdout = [](const char* buffer, size_t size) { std::cout.write(buffer, size); };

  auto read_result = read_file_buffered(file_path, on_data_print_to_stdout);
  if (read_result.is_err()) {
    spdlog::error("File reading failed: {}", read_result.error().error_message());
    return -1;
  }

  spdlog::info("Successfully displayed the content of the file");
  return 0;
}

/**
 * Save the current clipboard text to a file.
 * @return 0 on success, 1 on failure.
 */
int save_clipboard_to_file() {
  spdlog::info("Saving clipboard to file");
  std::string guid;
  aww::Result aww_result = aww::util::get_guid(guid);
  if (aww_result.is_failed()) {
    spdlog::error("Failed to get guid");
    return 1;
  }

  std::string clipboard_text{};
  if (!clip::get_text(clipboard_text)) {
    spdlog::error("Failed to get clipboard text");
    return 1;
  }

  fmt::print("Clipboard text: {}\n", clipboard_text);

  // display formatted character hex codes
  for (const auto& c : clipboard_text) {
    fmt::print("{:02x} ", static_cast<unsigned char>(c));
  }

  const std::string file_name = fmt::format("clipboard-{}.txt", guid);
  spdlog::info("Writing clipboard to file: '{}'", file_name);

  const std::vector<std::string> file_content{clipboard_text};
  auto write_result = aww::fs::write_lines(file_name, file_content);
  if (write_result.is_failed()) {
    spdlog::error("Failed to write clipboard to file");
    return 1;
  }
  spdlog::info("Clipboard saved to file");
  return 0;
}

/**
 * Set the clipboard text from a file.
 * @param file_name The name of the file to read from.
 * @return 0 on success, 1 on failure.
 */
int set_clipboard_from_file(const std::string& file_name) {
  spdlog::info("Setting clipboard from file");
  spdlog::info("Reading clipboard from file: '{}'", file_name);

  std::vector<std::string> file_content;
  auto read_result = aww::fs::read_lines(file_name, file_content);
  if (read_result.is_failed()) {
    spdlog::error("Failed to read clipboard from file");
    return 1;
  }

  std::string clipboard_text = aww::string::join(file_content, "\n");
  spdlog::info("Clipboard text: '{}'", clipboard_text);

  if (!clip::set_text(clipboard_text)) {
    spdlog::error("Failed to set clipboard text");
    return 1;
  }

  spdlog::info("Clipboard set from file");
  return 0;
}

/**
 * Save the command line arguments to a file.
 * @param cmd_args Command line arguments.
 * @return 0 on success, 1 on failure.
 */
int save_command_line_args_to_file(const std::vector<std::string>& cmd_args) {
  spdlog::info("Saving command line arguments to file");
  std::string guid;
  aww::Result aww_result = aww::util::get_guid(guid);
  if (aww_result.is_failed()) {
    spdlog::error("Failed to get guid");
    return 1;
  }

  std::vector<std::string> file_content;
  for (const auto& arg : cmd_args) {
    file_content.push_back(fmt::format("\"{}\"", arg));
    std::string hex_representation;
    for (const auto& c : arg) {
      hex_representation += fmt::format("{:02x} ", static_cast<unsigned char>(c));
    }
    file_content.push_back(fmt::format("Bytes: {}", hex_representation));
  }

  const std::string file_name = fmt::format("cmd-args-{}.txt", guid);
  spdlog::info("Writing command line arguments to file: '{}'", file_name);

  auto write_result = aww::fs::write_lines(file_name, file_content);
  if (write_result.is_failed()) {
    spdlog::error("Failed to write command line arguments to file");
    return 1;
  }
  spdlog::info("Command line arguments saved to file");
  return 0;
}

int aww_stuff_main([[maybe_unused]] const std::vector<std::string>& cmd_args,
                   [[maybe_unused]] aww_stuff_io_dependencies_interface& deps) {
  spdlog::info("aww stuff is a test tool to to do all kinds of stuff");

  constexpr auto save_clipboard_to_file_cmd = "save-clipboard-to-file";
  constexpr auto set_clipboard_from_file_cmd = "set-clipboard-from-file";
  constexpr auto save_command_line_args_to_file_cmd = "save-command-line-args-to-file";
  constexpr auto cat_cmd = "cat";

  if (cmd_args.size() >= 1 && cmd_args[0] == save_clipboard_to_file_cmd) {
    return save_clipboard_to_file();
  } else if (cmd_args.size() >= 2 && cmd_args[0] == set_clipboard_from_file_cmd) {
    return set_clipboard_from_file(cmd_args[1]);
  } else if (cmd_args.size() >= 1 && cmd_args[0] == save_command_line_args_to_file_cmd) {
    return save_command_line_args_to_file(cmd_args);
  } else if (cmd_args.size() >= 2 && cmd_args[0] == cat_cmd) {
    return cat_command(cmd_args[1]);
  } else {
    fmt::print("Usage: aww-stuff <command>\n");
    fmt::print("Commands:\n");
    fmt::print("  {} - Save the clipboard to a file\n", save_clipboard_to_file_cmd);
    fmt::print("  {} file_name - Set the clipboard from a file\n", set_clipboard_from_file_cmd);
    fmt::print("  {} - Save command line arguments to a file\n", save_command_line_args_to_file_cmd);
    fmt::print("  {} file_name - Display the content of a text file\n", cat_cmd);
  }
  return -1;
}

} // namespace aww::internal::aww_stuff

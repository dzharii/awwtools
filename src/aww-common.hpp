#pragma once
#ifndef AWW_COMMON_HPP
#define AWW_COMMON_HPP

#include <string>
#include <tuple>
#include <vector>

#include <chrono>
#include <cstdio>
#include <ctime>
#include <filesystem>
#include <functional>
#include <stdexcept>
#include <stdlib.h>
#include <string_view>
#include <thread>

#include <aww-collection/aww-collection.hpp>

// Windows tricks for aww::os::Proccess
#ifdef _WIN32
#define popen _popen
#define pclose _pclose
#define WEXITSTATUS
#endif

namespace aww {

// CONSTANTS
namespace constants {

// COMMON COMMANDLINE FLAGS
const std::string CMD_FLAG_NO_LOGGING = "--aww-no-logging";
const std::string CMD_FLAG_NO_NOTIFICATIONS = "--aww-no-notifications";
// The logic of one side effect is to only output the result and do nothing else.
// this includes CMD_FLAG_NO_LOGGING, CMD_FLAG_NO_NOTIFICATIONS, automatic clipboard copy
const std::string CMD_FLAG_NO_SIDE_EFFECTS = "--aww-no-side-effects";

// WELL KNOWN NAMES
const std::string AWW_SCRIPTS_FOLDER_NAME = "aww-scripts";
const std::string AWW_TOOLS_DOT_FOLDER_NAME = ".awwtools";

} // namespace constants

// COLLECTIONS

/**
 * @brief Removes a flag from the argument list. (aww tag #20250511a1)
 * @param args Argument list to modify.
 * @param flag Flag to remove.
 * @return True if flag was found and removed, false otherwise.
 */
bool erase_flag_from_args(std::vector<std::string>& args, const std::string& flag);

// RESULT
/**
 * @brief Represents the result of an operation. (aww tag #20250511a2)
 */
class Result {
public:
  /**
   * @brief Creates a successful result.
   */
  static Result ok() {
    return Result(true, std::string());
  }

  /**
   * @brief Creates a failed result with an error message.
   * @param error Error message.
   */
  static Result fail(const std::string& error) {
    return Result(false, error);
  }

  /**
   * @brief Checks if the result is successful.
   */
  bool is_ok() const {
    return m_success;
  }

  /**
   * @brief Checks if the result is a failure.
   */
  bool is_failed() const {
    return !m_success;
  }

  /**
   * @brief Returns the error message.
   */
  std::string error() const {
    return m_errorMessage;
  }

private:
  bool m_success;
  std::string m_errorMessage;

  Result(const bool isSuccess, const std::string& errorMessage) : m_success(isSuccess), m_errorMessage(errorMessage) {
  }
};

} // namespace aww

namespace aww::date {

/**
 * @brief Gets the current date in YYYY-MM-DD format. (aww tag #20250511a3)
 * @return Date string.
 */
std::string get_date_YYYYMMDD(void);
} // namespace aww::date

namespace aww::os {

enum class Platform {
  Unknown,
  Windows,
  Linux,
};

// OSPlatform
#if defined(_WIN32)
const Platform OSPlatform = Platform::Windows;
#elif defined(__linux__)
const Platform OSPlatform = Platform::Linux;
#else
const Platform OSPlatform = Platform::Unknown;
#endif

/**
 * @brief Checks if a file is executable. (aww tag #20250511a4)
 * @param path File path.
 * @return True if executable, false otherwise.
 */
bool can_execute(const std::filesystem::path& path);

/**
 * @brief Represents a process for command execution. (aww tag #20250511a5)
 */
class Proccess {
public:
  /**
   * @brief Constructs a Proccess object.
   */
  Proccess();
  /**
   * @brief Sets the callback for stdout.
   * @param Callback function.
   * @return Reference to this object.
   */
  Proccess& onStdOut(std::function<void(const std::string)>);
  /**
   * @brief Sets the callback for stderr.
   * @param Callback function.
   * @return Reference to this object.
   */
  Proccess& onStdErr(std::function<void(const std::string)>);
  /**
   * @brief Sets the callback for process exit.
   * @param Callback function.
   * @return Reference to this object.
   */
  Proccess& onExit(std::function<void(int)>);
  /**
   * @brief Runs the given command.
   * @param Command string.
   * @return Exit code.
   */
  int run(const std::string&);

private:
  std::function<void(std::string)> onStdOutCallback;
  std::function<void(std::string)> onStdErrCallback;
  std::function<void(int)> onExitCallback;

  static void defaultStdOutCallback(std::string);
  static void defaultStdErrCallback(std::string);
  static void defaultExitCallback(int);
};

/**
 * @brief Gets the command line arguments. (aww tag #20250511a6)
 * @return List of arguments.
 */
std::vector<std::string> get_command_line_args(void);
} // namespace aww::os

namespace aww::os::actions {
/**
 * @brief Launches a file using the OS default handler. (aww tag #20250511a7)
 * @param path File path.
 * @return Result of the operation.
 */
aww::Result launch_file(const std::string&);
/**
 * @brief Shows a system notification. (aww tag #20250511a8)
 * @param title Notification title.
 * @param message Notification message.
 * @return Result of the operation.
 */
aww::Result show_notification(const std::string&, const std::string&);
} // namespace aww::os::actions

namespace aww::os::env {
/**
 * @brief Gets the .awwtools folder path. (aww tag #20250511a9)
 * @return Path if found.
 */
std::optional<std::filesystem::path> get_aww_dot_folder(void);
/**
 * @brief Gets the aww-scripts folder path inside .awwtools. (aww tag #20250511aa)
 * @return Path if found.
 */
std::optional<std::filesystem::path> get_aww_dot_folder_aww_scripts_folder(void);
} // namespace aww::os::env

namespace aww::string {
/**
 * @brief Joins a vector of strings with a delimiter. (aww tag #20250511ab)
 * @param v Vector of strings.
 * @param delimiter Delimiter string.
 * @return Joined string.
 */
std::string join(const std::vector<std::string>&, const std::string&);
/**
 * @brief Pads a string on the left. (aww tag #20250511ac)
 * @param inp Input string.
 * @param padChar Padding character.
 * @param padLength Total length after padding.
 * @return Padded string.
 */
std::string left_padding(const std::string&, const char&, const size_t&);
/**
 * @brief Converts a string to uppercase. (aww tag #20250511ad)
 * @param inp Input string.
 * @return Uppercase string.
 */
std::string to_upper(const std::string&);
/**
 * @brief Converts a string to lowercase. (aww tag #20250511ae)
 * @param inp Input string.
 * @return Lowercase string.
 */
std::string to_lower(const std::string&);
/**
 * @brief Checks if a string ends with a suffix. (aww tag #20250511af)
 * @param str Input string.
 * @param suffix Suffix string.
 * @return True if ends with suffix.
 */
bool ends_with(const std::string& str, const std::string& suffix);
/**
 * @brief Converts a string to a valid identifier. (aww tag #20250511b0)
 * @param input Input string.
 * @return Valid identifier string.
 */
std::string to_valid_identifier(const std::string& input);
/**
 * @brief Trims whitespace from both ends of a string. (aww tag #20250511b1)
 * @param str Input string.
 * @return Trimmed string.
 */
std::string trim(std::string str);
} // namespace aww::string

namespace aww::fs {
/**
 * @brief Gets the current executable path. (aww tag #20250511b2)
 * @return Executable path.
 */
std::filesystem::path get_current_executable_path(void);

/**
 * @brief Gets the absolute path of the current directory. (aww tag #20250511b3)
 * @param path Output path.
 * @return Result of the operation.
 */
aww::Result get_current_directory_absolute_path(std::filesystem::path& path);

/**
 * @brief Checks if a path is a directory. (aww tag #20250511b4)
 * @param path Path to check.
 * @param outIsDirectory Output boolean.
 * @return Result of the operation.
 */
aww::Result is_directory(const std::filesystem::path& path, bool& outIsDirectory);

/**
 * @brief Creates directories recursively. (aww tag #20250511b5)
 * @param path Directory path.
 * @return Result of the operation.
 */
aww::Result create_directories(const std::filesystem::path& path);

/**
 * @brief Creates an empty file. (aww tag #20250511b6)
 * @param path File path.
 * @return Result of the operation.
 */
aww::Result create_empty_file(const std::filesystem::path& path);

/**
 * @brief Reads all lines from a file. (aww tag #20250511b7)
 * @param filePath File path.
 * @param outFileLines Output lines.
 * @return Result of the operation.
 */
aww::Result read_lines(const std::filesystem::path& filePath, std::vector<std::string>& outFileLines);

/**
 * @brief Reads an ASCII text file. (aww tag #20250511b8)
 * @param path File path.
 * @return File contents as string.
 */
std::string read_ascii_text_file(const std::filesystem::path&);

/**
 * @brief Writes lines to a file. (aww tag #20250511b9)
 * @param filePath File path.
 * @param lines Lines to write.
 * @return Result of the operation.
 */
aww::Result write_lines(const std::filesystem::path& filePath, const std::vector<std::string>& lines);

/**
 * @brief Normalizes a file path. (aww tag #20250511ba)
 * @param inputPath Input path string.
 * @return Normalized path string.
 */
std::string normalize_file_path(const std::string&);
} // namespace aww::fs

namespace aww::util {
/**
 * @brief Generates a GUID string. (aww tag #20250511bb)
 * @param out Output GUID string.
 * @return Result of the operation.
 */
aww::Result get_guid(std::string&);
} // namespace aww::util

namespace aww::draw {
class RgbaColor {
public:
  RgbaColor(int red, int green, int blue, int alpha) {
    if (red < 0 || red > 255) {
      throw std::invalid_argument("red must be between 0 and 255");
    }

    if (green < 0 || green > 255) {
      throw std::invalid_argument("green must be between 0 and 255");
    }

    if (blue < 0 || blue > 255) {
      throw std::invalid_argument("blue must be between 0 and 255");
    }

    if (alpha < 0 || alpha > 255) {
      throw std::invalid_argument("alpha must be between 0 and 255");
    }

    this->red = red;
    this->green = green;
    this->blue = blue;
    this->alpha = alpha;
  }

  int get_red() const {
    return red;
  }

  int get_green() const {
    return green;
  }

  int get_blue() const {
    return blue;
  }

  int get_alpha() const {
    return alpha;
  }

private:
  int red;
  int green;
  int blue;
  int alpha;
};
} // namespace aww::draw

#endif // AWW_COMMON_HPP

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

// CPMMON COMMANDLINE FLAGS
const std::string CMD_FLAG_NO_LOGGING = "--aww-no-logging";
const std::string CMD_FLAG_NO_NOTIFICATIONS = "--aww-no-notifications";

} // namespace constants

// CallTag struct definition
struct call_tag_t {
  constexpr explicit call_tag_t(std::uint64_t value) : value(value) {}

  const std::uint64_t value;
};

// Compile-time hash function
constexpr std::uint64_t _compiletime_hash(const char* str, std::uint64_t hash = 0,
                                          size_t index = 0) {
  return str[index] ? _compiletime_hash(str, (hash * 131) + str[index], index + 1) : hash;
}

// call_tag function with compile-time length check for string literals
template <size_t N> constexpr call_tag_t call_tag(const char (&str)[N]) {
  static_assert(
      N > 11, "Tag string must be at least 11 characters long."); // N includes the null terminator
  return call_tag_t(_compiletime_hash(str));
}

// COLLECTIONS

/**
 * @brief Remove a flag from the arguments
 * @param args The arguments to remove the flag from
 * @param flag The flag name to remove
 * @returns true if the flag was found and removed, false otherwise
 */
bool erase_flag_from_args(std::vector<std::string>& args, const std::string& flag);

// RESULT
class Result {
public:
  /* Create a successful result */
  static Result ok() { return Result(true, std::string()); }

  /* Create a failed result */
  static Result fail(const std::string& error) { return Result(false, error); }

  bool is_ok() const { return m_success; }

  /* Use hasValue to check if there is an error */
  bool is_failed() const { return !m_success; }
  std::string error() const { return m_errorMessage; }

private:
  bool m_success;
  std::string m_errorMessage;

  Result(const bool isSuccess, const std::string& errorMessage)
      : m_success(isSuccess), m_errorMessage(errorMessage) {}
};

} // namespace aww

namespace aww::date {

/** Get the current date in YYYY-MM-DD format
 * @return Date string
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

/** Check if file is executable
 * Returns false if file does not exist
 * @returns true if file is executable
 */
bool can_execute(const std::filesystem::path& path);

class Proccess {
public:
  Proccess();
  Proccess& onStdOut(std::function<void(const std::string)>);
  Proccess& onStdErr(std::function<void(const std::string)>);
  Proccess& onExit(std::function<void(int)>);
  int run(const std::string&);

private:
  std::function<void(std::string)> onStdOutCallback;
  std::function<void(std::string)> onStdErrCallback;
  std::function<void(int)> onExitCallback;

  static void defaultStdOutCallback(std::string);
  static void defaultStdErrCallback(std::string);
  static void defaultExitCallback(int);
};

std::vector<std::string> get_command_line_args(void);
} // namespace aww::os

namespace aww::os::actions {
aww::Result launch_file(const std::string&);
aww::Result show_notification(const std::string&, const std::string&);
} // namespace aww::os::actions

namespace aww::os::env {
aww::Result get_user_home_dir(std::filesystem::path&);
std::filesystem::path get_aww_dot_dir(void);
} // namespace aww::os::env

namespace aww::string {
std::string join(const std::vector<std::string>&, const std::string&);
std::string left_padding(const std::string&, const char&, const size_t&);
std::string to_upper(const std::string&);
std::string to_lower(const std::string&);
bool ends_with(const std::string& str, const std::string& suffix);
std::string to_valid_identifier(const std::string& input);
std::string trim(std::string str);
} // namespace aww::string

namespace aww::fs {
std::filesystem::path get_current_executable_path(void);

aww::Result get_current_directory_absolute_path(std::filesystem::path& path);

aww::Result is_directory(const std::filesystem::path& path, bool& outIsDirectory);

aww::Result create_directories(const std::filesystem::path& path);

aww::Result create_empty_file(const std::filesystem::path& path);

aww::Result read_lines(const std::filesystem::path& filePath,
                       std::vector<std::string>& outFileLines);

std::string read_ascii_text_file(const std::filesystem::path&);

aww::Result write_lines(const std::filesystem::path& filePath,
                        const std::vector<std::string>& lines);

std::string normalize_file_path(const std::string&);
} // namespace aww::fs

namespace aww::util {
aww::Result get_guid(std::string&);
}

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

  int get_red() const { return red; }

  int get_green() const { return green; }

  int get_blue() const { return blue; }

  int get_alpha() const { return alpha; }

private:
  int red;
  int green;
  int blue;
  int alpha;
};
} // namespace aww::draw

#endif // AWW_COMMON_HPP

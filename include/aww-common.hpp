#pragma once
#ifndef AWW_COMMON_HPP
#define AWW_COMMON_HPP

#include <string>
#include <vector>
#include <tuple>

#include <cstdio>
#include <ctime>
#include <stdlib.h>
#include <filesystem>
#include <functional>
#include <chrono>
#include <thread>

// Windows tricks for aww::os::Proccess
#ifdef _WIN32
#define popen _popen
#define pclose _pclose
#define WEXITSTATUS
#endif

namespace aww
{

  const int resultPos = 0;
  const int errorPos = 1;

  typedef std::tuple<bool, std::string> result_t;

  bool failed(const result_t&);
  bool succeeded(const result_t&);
  std::string make_error(const std::string&, const result_t&);

} // namespace aww

namespace aww::date
{

  /** Get the current date in YYYY-MM-DD format
   * @return Date string
   */
  std::string getDateYYYYMMDD(void);
}

namespace aww::os
{
  enum class Platform
  {
    Unknown,
    Windows,
    Linux,
    MacOS
  };

  /** Get the current platform
  * @return Platform enum
  */
  Platform getPlatform(void);

  /** Check if file is executable
   * Returns false if file does not exist
   * @returns true if file is executable
  */
  bool canExecute(const std::filesystem::path &path);

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

  std::vector<std::string> getCommandLineArgs(void);
}

namespace aww::os::actions
{
  aww::result_t launchFile(const std::string &);
  aww::result_t showNotification(const std::string &, const std::string &);
}

namespace aww::os::env
{
  aww::result_t getUserHomeDir(std::filesystem::path &);
  std::filesystem::path getAwwDotDir(void);
}

namespace aww::string
{
  std::string join(const std::vector<std::string> &, const std::string &);
}

namespace aww::fs {
  std::filesystem::path getCurrentExecutablePath(void);

  /// @brief reads a text file at the given path and returns its contents as a string.
  /// @return file contents as a string
  std::string readAsciiTextFile(const std::filesystem::path &);
}

#endif // AWW_COMMON_HPP

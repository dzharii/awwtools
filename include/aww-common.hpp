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
  class Result {
  public:

    /* Create a successful result */
    static Result ok() {
      return Result(true, std::string());
    }

    /* Create a failed result */
    static Result fail(const std::string& error) {
      return Result(false, error);
    }

    bool isOk() const {
      return m_success;
    }

    /* Use hasValue to check if there is an error */
    bool isFailed() const {
      return !m_success;
    }
    std::string error() const {
      return m_errorMessage;
    }

  private:
    bool m_success;
    std::string m_errorMessage;

    Result(const bool isSuccess, const std::string& errorMessage) :
      m_success(isSuccess), m_errorMessage(errorMessage) {}
  };

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

  std::string escapeCommandLineArgs(const std::string);

  enum class Platform
  {
    Unknown,
    Windows,
    Linux,
    MacOS
  };

  // OSPlatform
#if defined(_WIN32)
    const Platform OSPlatform = Platform::Windows;
#elif defined(__linux__)
    const Platform OSPlatform = Platform::Linux;
#elif defined(__APPLE__)
    const Platform OSPlatform = Platform::MacOS;
#else
    const Platform OSPlatform = Platform::Unknown;
#endif

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
  aww::Result launchFile(const std::string &);
  aww::Result showNotification(const std::string &, const std::string &);
}

namespace aww::os::env
{
  aww::Result getUserHomeDir(std::filesystem::path &);
  std::filesystem::path getAwwDotDir(void);
}

namespace aww::string
{
  std::string join(const std::vector<std::string> &, const std::string &);
  std::string leftPadding(const std::string &, const char &, const size_t &);
}

namespace aww::fs {
  std::filesystem::path getCurrentExecutablePath(void);

  /// @brief reads a text file at the given path and returns its contents as a string.
  /// @return file contents as a string
  std::string readAsciiTextFile(const std::filesystem::path &);

  std::string normalizeFilePath(const std::string &);
}

namespace aww::util
{
   aww::Result getGuid(std::string &);
}

#endif // AWW_COMMON_HPP

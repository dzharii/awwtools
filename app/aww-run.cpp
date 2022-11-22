// Executables must have the following defined if the library contains
// doctest definitions. For builds with this disabled, e.g. code shipped to
// users, this can be left out.
#ifdef ENABLE_DOCTEST_IN_LIBRARY
#define DOCTEST_CONFIG_IMPLEMENT
#include "doctest/doctest.h"
#endif

#include <iostream>
#include <string>
#include <vector>
#include <filesystem>
#include <map>
#include <array>

#include "exampleConfig.h"
#include "example.hpp"
#include "os-exec.hpp"
#include "aww-common.hpp"

namespace fs = std::filesystem;

/* Attempt to find the path to the executable.
*/
aww::result_t findScript(const std::string &, fs::path &);

int main(int argc, char **argv)
{
  std::cout << "The AwwTools v"
            << PROJECT_VERSION_MAJOR
            << "."
            << PROJECT_VERSION_MINOR
            << "."
            << PROJECT_VERSION_PATCH
            << "."
            << PROJECT_VERSION_TWEAK
            << std::endl;
  std::cout << "Embrace the Aww!" << std::endl;

  if (argc < 2)
  {
    std::cout << "No arguments provided" << std::endl;
    return 1;
  }

  std::vector<std::string> cmdArgs(argv, argv + argc);
  cmdArgs.erase(cmdArgs.begin()); // remove first element

  aww::os::Proccess proc;
  int exitCode = 0;
  proc.onStdOut([](std::string line) {
    char &lastChar = line.back();
    std::string endl = lastChar == '\n' ? "" : "\n";
    std::cout << line <<  endl;
  });
  proc.onStdErr([](std::string line) {
    char &lastChar = line.back();
    std::string endl = lastChar == '\n' ? "" : "\n";
    std::cout << line <<  endl;
  });
  proc.onExit([&](int code) {
    std::cout << "Exit code: " << code << std::endl;
    exitCode = code;
  });

  std::string cmd = aww::string::join(cmdArgs, " ");
  std::cout << "Running command: " << cmd << std::endl;

  // measure time
  auto start = std::chrono::high_resolution_clock::now();

  proc.run(cmd);

  // print time
  auto end = std::chrono::high_resolution_clock::now();
  auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(end - start);
  std::cout << "Command took " << duration.count() << "ms" << std::endl;

  if (exitCode != 0) {
    aww::os::actions::showNotification("aww run", "Failed to run command");
  }
  aww::os::actions::showNotification("aww run", "The command finished successfully");

  return exitCode;
}


aww::result_t findScriptWindows(const std::string &scriptName, fs::path &outScriptPath) {
  const fs::path currentDir = fs::absolute(fs::current_path());
  const fs::path awwScriptsDir = currentDir / "aww-scripts";
  const fs::path awwDir = currentDir / "aww";

  const fs::path batCurrentDirPath = currentDir / (scriptName + ".bat");
  const fs::path cmdCurrentDirPath = currentDir / (scriptName + ".cmd");
  const fs::path ps1CurrentDirPath = currentDir / (scriptName + ".ps1");

  const fs::path batAwwScriptsPath = awwScriptsDir / (scriptName + ".bat");
  const fs::path cmdAwwScriptsPath = awwScriptsDir / (scriptName + ".cmd");
  const fs::path ps1AwwScriptsPath = awwScriptsDir / (scriptName + ".ps1");

  const fs::path batAwwPath = awwDir / (scriptName + ".bat");
  const fs::path cmdAwwPath = awwDir / (scriptName + ".cmd");
  const fs::path ps1AwwPath = awwDir / (scriptName + ".ps1");

  const size_t totalDirItems = 3 * 3;

  std::array<fs::path, totalDirItems> lookupPath = {
    batCurrentDirPath,
    cmdCurrentDirPath,
    ps1CurrentDirPath,

    batAwwScriptsPath,
    cmdAwwScriptsPath,
    ps1AwwScriptsPath,

    batAwwPath,
    cmdAwwPath,
    ps1AwwPath,
  };

  for (const fs::path &path : lookupPath) {
    if (fs::exists(path)) {
      outScriptPath = path;
      return std::make_tuple(true, "");
    }
  }
  return std::make_tuple(false, "Script not found");
}

aww::result_t findScriptLinux(const std::string &scriptName, fs::path &outScriptPath) {
  const fs::path currentDir = fs::absolute(fs::current_path());
  const fs::path awwScriptsDir = currentDir / "aww-scripts";
  const fs::path awwDir = currentDir / "aww";

  const fs::path shCurrentDirPath = currentDir / (scriptName + ".sh");
  const fs::path shAwwScriptsPath = awwScriptsDir / (scriptName + ".sh");
  const fs::path shAwwPath = awwDir / (scriptName + ".sh");

  const fs::path emptyCurrentDirPath = currentDir / scriptName;
  const fs::path emptyAwwScriptsPath = awwScriptsDir / scriptName;
  const fs::path emptyPath = awwDir / scriptName;

  const std::array<fs::path, 3> shLookupPath = {
    shCurrentDirPath,
    shAwwScriptsPath,
    shAwwPath,
  };

  const std::array<fs::path, 3> emptyLookupPath = {
    emptyCurrentDirPath,
    emptyAwwScriptsPath,
    emptyPath,
  };

  for (const fs::path &path : shLookupPath) {
    if (fs::exists(path)) {
      outScriptPath = path;
      return std::make_tuple(true, "");
    }
  }

  for (const fs::path &path : emptyLookupPath) {
    if (fs::exists(path) && aww::os::canExecute(path)) {
      outScriptPath = path;
      return std::make_tuple(true, "");
    }
  }

  return std::make_tuple(false, "Script not found");
}

aww::result_t findScriptMacOS(const std::string &scriptName, fs::path &outScriptPath) {
  return findScriptLinux(scriptName, outScriptPath);
}

aww::result_t findScript(const std::string &scriptName, fs::path &outScriptPath) {

  aww::os::Platform platform = aww::os::getPlatform();

  switch (platform) {
    case aww::os::Platform::Windows:
      return findScriptWindows(scriptName, outScriptPath);
    case aww::os::Platform::Linux:
      return findScriptLinux(scriptName, outScriptPath);
    case aww::os::Platform::MacOS:
      return findScriptMacOS(scriptName, outScriptPath);
    default:
      return std::make_tuple(false, "Unknown platform");
  }
}





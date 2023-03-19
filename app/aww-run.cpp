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
#include "aww-common.hpp"

namespace fs = std::filesystem;

aww::Result findScriptWindows(const std::string&, fs::path&);
aww::Result findScriptLinux(const std::string&, fs::path&);
aww::Result findScriptMacOS(const std::string&, fs::path&);

/* Attempt to find the path to the executable.
*/
aww::Result findScript(const std::string &, fs::path &);

int main(int argc, char **argv)
{
  if (argc < 2)
  {
    std::cout << "No arguments provided" << "\n";
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
    std::cout << "Exit code: " << code << "\n";
    exitCode = code;
  });

  // aww command is a shell script name without extension
  // so that we can have "build.sh" and "build.bat" in the same folder
  // and the correct one will be selected based on the OS
  // Sample:
  //        aww run build
  std::string& awwCommand = cmdArgs[0];
  fs::path maybeScriptPath;

  aww::Result scriptFound = findScript(awwCommand, maybeScriptPath);
  if (scriptFound.isOk())
  {
    std::cout << "Found script: " << maybeScriptPath << "\n";
    std::string scriptExtension = maybeScriptPath.extension().string();

    bool isPowerShell = scriptExtension == ".ps1" || scriptExtension == ".PS1";
    bool isBash = scriptExtension == ".sh" || scriptExtension == ".SH";
    const aww::os::Platform platform = aww::os::OSPlatform;

    switch (platform)
    {
      case aww::os::Platform::Windows:
        if (isPowerShell)
        {
          awwCommand = "powershell.exe -executionpolicy unrestricted -File \"" + maybeScriptPath.string() + "\"";
        }
        else
        {
          awwCommand = maybeScriptPath.string();
        }
        break;
      case aww::os::Platform::Linux:
        if (isPowerShell)
        {
          awwCommand = "pwsh -File \"" + maybeScriptPath.string() + "\"";
        }
        else if (isBash)
        {
          awwCommand = "bash \"" + maybeScriptPath.string() + "\"";
        }
        else
        {
          awwCommand = maybeScriptPath.string();
        }
        break;
      case aww::os::Platform::Unknown:
        std::cout << "Unknown platform" << "\n";
        return 1;
    }
  }
  else
  {
    std::cout << "Aww command was not found: " << awwCommand << "\n";
  }

  std::string cmd = aww::string::join(cmdArgs, " ");
  std::cout << "Running command: " << cmd << "\n";

  // measure time
  auto start = std::chrono::high_resolution_clock::now();

  proc.run(cmd);

  // print time
  auto end = std::chrono::high_resolution_clock::now();
  auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(end - start);
  std::cout << "Command took " << duration.count() << "ms" << "\n";

  if (exitCode != 0) {
    aww::os::actions::showNotification("aww run", "Failed to run command");
  }
  aww::os::actions::showNotification("aww run", "The command finished successfully");

  return exitCode;
}


aww::Result findScriptWindows(const std::string &scriptName, fs::path &outScriptPath) {
  // check if scriptName is not empty
  if (scriptName.empty()) {
    return aww::Result::fail("Script name is empty");
  }

  const fs::path currentDir = fs::absolute(fs::current_path());
  const fs::path awwScriptsDir = currentDir / "aww-scripts";
  const fs::path awwDotScriptsDir = aww::os::env::getAwwDotDir() / "aww-scripts";
  const fs::path awwDir = currentDir / "aww";

  const fs::path batCurrentDirPath = currentDir / (scriptName + ".bat");
  const fs::path cmdCurrentDirPath = currentDir / (scriptName + ".cmd");
  const fs::path ps1CurrentDirPath = currentDir / (scriptName + ".ps1");

  const fs::path batAwwScriptsPath = awwScriptsDir / (scriptName + ".bat");
  const fs::path cmdAwwScriptsPath = awwScriptsDir / (scriptName + ".cmd");
  const fs::path ps1AwwScriptsPath = awwScriptsDir / (scriptName + ".ps1");

  const fs::path batAwwDotScriptsPath = awwDotScriptsDir / (scriptName + ".bat");
  const fs::path cmdAwwDotScriptsPath = awwDotScriptsDir / (scriptName + ".cmd");
  const fs::path ps1AwwDotScriptsPath = awwDotScriptsDir / (scriptName + ".ps1");

  const fs::path batAwwPath = awwDir / (scriptName + ".bat");
  const fs::path cmdAwwPath = awwDir / (scriptName + ".cmd");
  const fs::path ps1AwwPath = awwDir / (scriptName + ".ps1");

  const size_t totalDirItems = 3 * 4;

  std::array<fs::path, totalDirItems> lookupPath = {
    batCurrentDirPath,
    cmdCurrentDirPath,
    ps1CurrentDirPath,

    batAwwScriptsPath,
    cmdAwwScriptsPath,
    ps1AwwScriptsPath,

    batAwwDotScriptsPath,
    cmdAwwDotScriptsPath,
    ps1AwwDotScriptsPath,

    batAwwPath,
    cmdAwwPath,
    ps1AwwPath,
  };

  for (const fs::path &path : lookupPath) {
    if (fs::exists(path)) {
      outScriptPath = path;
      return aww::Result::ok();
    }
  }
  return aww::Result::fail("Script not found");
}

aww::Result findScriptLinux(const std::string &scriptName, fs::path &outScriptPath) {
  if (scriptName.empty()) {
    return aww::Result::fail("Script name is empty");
  }

  const fs::path currentDir = fs::absolute(fs::current_path());
  const fs::path awwScriptsDir = currentDir / "aww-scripts";
  const fs::path awwDotScriptsDir = aww::os::env::getAwwDotDir() / "aww-scripts";
  const fs::path awwDir = currentDir / "aww";

  const fs::path shCurrentDirPath = currentDir / (scriptName + ".sh");
  const fs::path shAwwScriptsPath = awwScriptsDir / (scriptName + ".sh");
  const fs::path shAwwDotScriptsPath = awwDotScriptsDir / (scriptName + ".sh");
  const fs::path shAwwPath = awwDir / (scriptName + ".sh");

  const fs::path ps1CurrentDirPath = currentDir / (scriptName + ".ps1");
  const fs::path ps1AwwScriptsPath = awwScriptsDir / (scriptName + ".ps1");
  const fs::path ps1AwwDotScriptsPath = awwDotScriptsDir / (scriptName + ".ps1");
  const fs::path ps1AwwPath = awwDir / (scriptName + ".ps1");

  const fs::path emptyCurrentDirPath = currentDir / scriptName;
  const fs::path emptyAwwScriptsPath = awwScriptsDir / scriptName;
  const fs::path emptyAwwDotScriptsPath = awwScriptsDir / scriptName;
  const fs::path emptyPath = awwDir / scriptName;

  const std::array<fs::path, 8> shLookupPath = {
    shCurrentDirPath,
    shAwwScriptsPath,
    shAwwDotScriptsPath,
    shAwwPath,

    ps1CurrentDirPath,
    ps1AwwScriptsPath,
    ps1AwwDotScriptsPath,
    ps1AwwPath,
  };

  const std::array<fs::path, 4> emptyLookupPath = {
    emptyCurrentDirPath,
    emptyAwwScriptsPath,
    emptyAwwDotScriptsPath,
    emptyPath,
  };

  for (const fs::path &path : shLookupPath) {
    if (fs::exists(path)) {
      outScriptPath = path;
      return aww::Result::ok();
    }
  }

  for (const fs::path &path : emptyLookupPath) {
    if (fs::exists(path) && aww::os::canExecute(path)) {
      outScriptPath = path;
      return aww::Result::ok();
    }
  }

  return aww::Result::fail("Script not found");
}

aww::Result findScript(const std::string &scriptName, fs::path &outScriptPath) {
  if (scriptName.empty()) {
    return aww::Result::fail("Script name is empty");
  }

  const aww::os::Platform platform = aww::os::OSPlatform;

  switch (platform) {
    case aww::os::Platform::Windows:
      return findScriptWindows(scriptName, outScriptPath);
    case aww::os::Platform::Linux:
      return findScriptLinux(scriptName, outScriptPath);
    default:
      return aww::Result::fail("Unknown platform");
  }
}





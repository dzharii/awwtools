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

#include "exampleConfig.h"
#include "aww-common.hpp"

int main(int argc, char **argv)
{
  if (argc < 2)
  {
    std::cout << "No arguments provided" << "\n";
    return 1;
  }

  std::vector<std::string> cmdArgs(argv, argv + argc);
  cmdArgs.erase(cmdArgs.begin()); // remove first element
  std::filesystem::path awwExecutablePath = aww::fs::getCurrentExecutablePath();
  std::filesystem::path awwExecutableDir = std::filesystem::absolute(awwExecutablePath.parent_path());

  std::cout << "awwExecutablePath: " << awwExecutablePath << "\n";
  std::cout << "awwExecutableDir: " << awwExecutableDir << "\n";

  std::string maybeAwwExecutable = (awwExecutableDir / "aww").string();
  bool isAwwExecutable = false;

  auto itCmdArg = cmdArgs.begin();
  for (; itCmdArg != cmdArgs.end(); ++itCmdArg)
  {
    maybeAwwExecutable += "-" + *itCmdArg;
    if (std::filesystem::exists(maybeAwwExecutable))
    {
      isAwwExecutable = true;
      break;
    }
    else if (std::filesystem::exists(maybeAwwExecutable + ".exe"))
    {
      isAwwExecutable = true;
      maybeAwwExecutable += ".exe";
      break;
    }
  }

  if (!isAwwExecutable)
  {
      std::cout << "No aww executable found" << "\n";
      return 1;
  }
  // slice itCmdArg to end
  ++itCmdArg;
  std::vector<std::string> awwExecutableArgs(itCmdArg, cmdArgs.end());
  std::string awwExecutableArgsStr = aww::string::join(awwExecutableArgs, " ");

  std::filesystem::path executablePath(maybeAwwExecutable);
  std::string cmd = std::filesystem::absolute(executablePath).string() +
                    " " +
                    awwExecutableArgsStr;
  std::cout << "Executing: "
            << cmd
            << "\n";

  return system(cmd.c_str());
}

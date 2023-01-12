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

#include "aww-common.hpp"

namespace fs = std::filesystem;


int main(int argc, char **argv)
{
  std::vector<std::string> cmdArgs(argv, argv + argc);
  cmdArgs.erase(cmdArgs.begin()); // remove first element

  std::string optionalCommandLine = aww::string::join(cmdArgs, "-");
  // current folder absolute path
  fs::path currentFolder = fs::absolute(fs::current_path());

  // open new terminal with bash in new window
  std::string launchTerm;
  std::string escapedCommandLine = aww::os::escapeCommandLineArgs(optionalCommandLine);
  if (aww::os::getPlatform() == aww::os::Platform::Windows)
  {
    launchTerm = "start cmd.exe @cmd /k " + escapedCommandLine;
  } else {
    // escape optionalCommandLine for bash
    launchTerm = "x-terminal-emulator -e bash " + escapedCommandLine;
  }
  int retValue = std::system(launchTerm.c_str());
  if (retValue != 0)
  {
    std::cerr << "Error: " << retValue << std::endl;
  }
  return retValue;
}


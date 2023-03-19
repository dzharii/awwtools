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


int main(int, char**)
{
  // open new terminal with bash in new window
  std::string launchTerm;

  const aww::os::Platform platform = aww::os::OSPlatform;

  if constexpr (platform == aww::os::Platform::Windows)
  {
    launchTerm = "start cmd.exe @cmd /k";
  } else if constexpr (platform == aww::os::Platform::Linux){
    launchTerm = "x-terminal-emulator -e bash &";
  } else {
    std::cerr << "Error: Unsupported platform" << "\n";
    return 1;
  }
  int retValue = std::system(launchTerm.c_str());
  if (retValue != 0)
  {
    std::cerr << "Error: " << retValue << "\n";
  }
  return retValue;
}


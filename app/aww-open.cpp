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
  if (argc < 2)
  {
    std::cout << "No arguments provided" << std::endl;
    return 1;
  }

  if (argc > 2) {
    std::cout << "Too many arguments provided. Expected 1: file to open" << std::endl;
    return 1;
  }

  std::string fileToOpen = argv[1];
  aww::result_t launchFileRes = aww::os::actions::launchFile(fileToOpen);
  if (aww::failed(launchFileRes))
  {
    std::cout << aww::make_error("Failed to launch file", launchFileRes) << std::endl;
    aww::os::actions::showNotification("aww open", "Failed to open file: " + fileToOpen);
    return 1;
  }
  std::cout << "Launched file" << std::endl;
  aww::os::actions::showNotification("aww open", "The file was opened");
  return 0;
}


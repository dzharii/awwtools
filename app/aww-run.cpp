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
#include "example.hpp"
#include "os-exec.hpp"
#include "aww-common.hpp"

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
  return 0;
}

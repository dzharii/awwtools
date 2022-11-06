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

  aww::os::Proccess proc;
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
  proc.onExit([](int code) {
    std::cout << "Exit code: " << code << std::endl;
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

  return 0;
}

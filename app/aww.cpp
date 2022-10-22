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
#include "clip.h"

int main(int argc, char** argv) {
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

  if (argc < 2) {
    std::cout << "No arguments provided" << std::endl;
    return 1;
  }


  // argv to vector of strings
  std::vector<std::string> cmdArgs(argv, argv + argc);
  cmdArgs.erase(cmdArgs.begin()); // remove first element

  std::string maybeAwwExecutable = "aww";
  bool isAwwExecutable = false;

  auto itCmdArg = cmdArgs.begin();
  for (; itCmdArg != cmdArgs.end(); ++itCmdArg) {
    maybeAwwExecutable += "-" + *itCmdArg;
    if (std::filesystem::exists(maybeAwwExecutable)) {
      isAwwExecutable = true;
      break;
    } else if (std::filesystem::exists(maybeAwwExecutable + ".exe")) {
      isAwwExecutable = true;
      maybeAwwExecutable += ".exe";
      break;
    }
  }

  if (isAwwExecutable) {
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
              << std::endl;

    return system(cmd.c_str());
  }
  std::cout << "No aww executable found" << std::endl;
  return 1;

  /* Commented code should go to aww-date
  // get current date yyyy-mm-dd as string
  std::string date = aww::date::getDateYYYYMMDD();


  clip::set_text(date);
  std::cout << "Copied to clipboard: "
            << date
            << std::endl;

  // Bring in the dummy class from the example source,
  // just to show that it is accessible from main.cpp.
  Dummy d = Dummy();
  return d.doSomething() ? 0 : -1;
  */
}

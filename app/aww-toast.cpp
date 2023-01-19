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
  std::vector<std::string> cmdArgs(argv, argv + argc);
  cmdArgs.erase(cmdArgs.begin()); // remove first element
  std::string message = "Hey!";
  if (cmdArgs.size() > 0)
  {
    message = aww::string::join(cmdArgs, " ");
  }
  auto result =  aww::os::actions::showNotification("AwwTools", message);
  if (aww::succeeded(result)) {
    std::cout << "Notification sent" << std::endl;
  } else {
    std::cout << "Failed to send notification" << std::endl;
  }

  return 0;
}

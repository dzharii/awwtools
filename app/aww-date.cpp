// Executables must have the following defined if the library contains
// doctest definitions. For builds with this disabled, e.g. code shipped to
// users, this can be left out.
#ifdef ENABLE_DOCTEST_IN_LIBRARY
#define DOCTEST_CONFIG_IMPLEMENT
#include "doctest/doctest.h"
#endif

#include <iostream>
#include <string>
#include <regex>

#include "clip.h"
#include "aww-common.hpp"

int main(int argc, char **argv)
{
  std::string currentDate = aww::date::getDateYYYYMMDD();

  std::vector<std::string> cmdArgs(argv, argv + argc);
  cmdArgs.erase(cmdArgs.begin()); // remove first element

  std::string fileName = aww::string::join(cmdArgs, "-");
  std::regex replaceFilenameUnsafeChars("[^\\._a-zA-Z0-9-]");
  std::string safeFileName = std::regex_replace(fileName, replaceFilenameUnsafeChars, "-");

  std::string result = currentDate;

  if (!safeFileName.empty()) {
    result = result + "-" + safeFileName;
  }
  if (clip::set_text(result)) {
    std::cout << "Copied to clipboard: " << result << std::endl;
    aww::os::actions::showNotification("aww date", "The date has been copied to the clipboard");
  } else {
    std::cout << "Failed to copy to clipboard: " << result << std::endl;
    aww::os::actions::showNotification("aww date", "Failed to copy the date to the clipboard");
  }

  std::cout << "Result:"
            << std::endl
            << result
            << std::endl;

  return 0;
}

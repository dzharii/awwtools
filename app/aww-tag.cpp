// Executables must have the following defined if the library contains
// doctest definitions. For builds with this disabled, e.g. code shipped to
// users, this can be left out.
#ifdef ENABLE_DOCTEST_IN_LIBRARY
#define DOCTEST_CONFIG_IMPLEMENT
#include "doctest/doctest.h"
#endif

#include <iostream>
#include <string>

#include "clip.h"
#include "aww-common.hpp"
#include "internal/aww-tag.hpp"

int main(int argc, char **argv)
{

  size_t length = 11;
  int userInput = 0;
  bool useDefaultInput = argc < 2;
  if (!useDefaultInput) {
    try {
      userInput = std::stoi(argv[1]);
      if (userInput < 1) {
        std::cout << "Invalid argument: '" << userInput << "', must be a positive number\n";
        return 1;
      }
      length = static_cast<size_t>(userInput);
    } catch (const std::invalid_argument&) {
      std::cout << "Invalid argument, not a number\n";
      return 1;
    } catch (const std::out_of_range&) {
      std::cout << "Invalid argument, the number is too big\n";
      return 1;
    }
  }

  std::string tag = aww::internal::aww_tag::makeTag(length);

  if (clip::set_text(tag))
  {
    std::cout << "Copied to clipboard:\n" << tag << "\n";
    aww::os::actions::showNotification("aww tag", "The tag has been copied to the clipboard");
  }
  else
  {
    std::cout << "Failed to copy to clipboard:\n" << tag << "\n";
    aww::os::actions::showNotification("aww tag", "Failed to copy the tag to the clipboard");
  }

  return 0;
}


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
  bool useDefaultInput = argc < 2;
  if (!useDefaultInput) {
    try {
      length = std::stoul(argv[1]);
    } catch (const std::invalid_argument& e) {
      std::cout << "Invalid argument: " << e.what() << "\n";
      return 1;
    } catch (const std::out_of_range& e) {
      std::cout << "Out of range: " << e.what() << "\n";
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


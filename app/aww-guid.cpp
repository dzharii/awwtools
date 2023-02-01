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

int main(void)
{

  std::string guid;
  aww::result_t result = aww::util::getGuid(guid);

  if (aww::failed(result)) {
    std::cout << aww::makeError("Failed to get GUID", result) << std::endl;
    return 1;
  }

  if (clip::set_text(guid)) {
    std::cout << "Copied to clipboard: " << guid << std::endl;
    aww::os::actions::showNotification("aww guid", "The guid has been copied to the clipboard");
  } else {
    std::cout << "Failed to copy to clipboard: " << guid << std::endl;
    aww::os::actions::showNotification("aww guid", "Failed to copy the guid to the clipboard");
  }

  std::cout << "Result:"
            << std::endl
            << guid
            << std::endl;

  return 0;
}

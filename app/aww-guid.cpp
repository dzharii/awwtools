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
  aww::Result result = aww::util::getGuid(guid);

  if (result.isFailed()) {
    std::cout << "Failed to get GUID" <<  result.error() << "\n";
    return 1;
  }

  if (clip::set_text(guid)) {
    std::cout << "Copied to clipboard: " << guid << "\n";
    aww::os::actions::showNotification("aww guid", "The guid has been copied to the clipboard");
  } else {
    std::cout << "Failed to copy to clipboard: " << guid << "\n";
    aww::os::actions::showNotification("aww guid", "Failed to copy the guid to the clipboard");
  }

  std::cout << "Result:"
            << "\n"
            << guid
            << "\n";

  return 0;
}

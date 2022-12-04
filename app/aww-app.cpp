// Executables must have the following defined if the library contains
// doctest definitions. For builds with this disabled, e.g. code shipped to
// users, this can be left out.
#ifdef ENABLE_DOCTEST_IN_LIBRARY
#define DOCTEST_CONFIG_IMPLEMENT
#include "doctest/doctest.h"
#endif

#include <iostream>
#include <string>

#include "aww-common.hpp"
#include "webview.h"

int main(int argc, char **argv)
{
  std::vector<std::string> cmdArgs(argv, argv + argc);
  cmdArgs.erase(cmdArgs.begin()); // remove first element

  std::cout << "Hello World!" << std::endl;

  std::string myCommandlineArgs = aww::string::join(aww::os::getCommandLineArgs(), "<br />");
  std::cout << "myCommandlineArgs: " << myCommandlineArgs << std::endl;

  webview::webview w(false, nullptr);
  w.set_title("Basic Example");
  w.set_size(480, 320, WEBVIEW_HINT_NONE);
  w.set_html("Thanks for using webview! <br/> " + myCommandlineArgs);
  w.run();

  return 0;
}

// Executables must have the following defined if the library contains
// doctest definitions. For builds with this disabled, e.g. code shipped to
// users, this can be left out.
#ifdef ENABLE_DOCTEST_IN_LIBRARY
#define DOCTEST_CONFIG_IMPLEMENT
#include "doctest/doctest.h"
#endif

#include <iostream>
#include <stdlib.h>

#include "exampleConfig.h"
#include "example.hpp"
#include "os-exec.hpp"

/*
 * Simple main program that demontrates how access
 * CMake definitions (here the version number) from source code.
 */
int main() {
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
  std::cout << exec("echo \"Hello World\"") << std::endl;
  std::system("cat ../LICENSE");


  // Bring in the dummy class from the example source,
  // just to show that it is accessible from main.cpp.
  Dummy d = Dummy();
  return d.doSomething() ? 0 : -1;
}

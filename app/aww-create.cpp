#include <iostream>
#include <string>

#include "aww-common.hpp"

int main(int argc, char **argv)
{
  std::vector<std::string> cmdArgs(argv, argv + argc);
  cmdArgs.erase(cmdArgs.begin()); // remove first element
  


  return 0;
}

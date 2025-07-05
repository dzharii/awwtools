#include <iostream>
#include <string>
#include <vector>

#include "aww-common.hpp"
#include "aww-spdlog-configuration.hpp"
#include "internal/aww-term.hpp"

namespace aww::internal::aww_term {
int aww_term_main(const std::vector<std::string>& cmdArgs) {
  init_default_spdlog_configuration("aww-term");
  std::string cmdArgsArgsAsString = aww::string::join(cmdArgs, " ");

  // open new terminal with bash in new window
  std::string launchTerm;

  const aww::os::Platform platform = aww::os::OSPlatform;

  if constexpr (platform == aww::os::Platform::Windows) {
    launchTerm = "start cmd.exe /k " + cmdArgsArgsAsString;
  } else if constexpr (platform == aww::os::Platform::Linux) {
    launchTerm = "x-terminal-emulator -e bash -c \"" + cmdArgsArgsAsString + "\"&";
  } else {
    std::cerr << "Error: Unsupported platform"
              << "\n";
    return 1;
  }
  int retValue = std::system(launchTerm.c_str());
  if (retValue != 0) {
    std::cerr << "Error: " << retValue << "\n";
  }
  return retValue;
}
} // namespace aww::internal::aww_term

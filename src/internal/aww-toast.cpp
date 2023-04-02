#include <iostream>
#include <string>
#include <vector>

#include "aww-common.hpp"
#include "internal/aww-toast.hpp"

namespace aww::internal::aww_toast
{
  int aww_toast_main(int argc, char **argv)
  {
    std::vector<std::string> cmdArgs(argv, argv + argc);
    cmdArgs.erase(cmdArgs.begin()); // remove first element
    std::string message = "Hey!";
    if (cmdArgs.size() > 0)
    {
      message = aww::string::join(cmdArgs, " ");
    }
    aww::Result result = aww::os::actions::showNotification("AwwTools", message);
    if (result.isOk())
    {
      std::cout << "Notification sent"
                << "\n";
    }
    else
    {
      std::cout << "Failed to send notification"
                << "\n";
    }

    return 0;
  }
}

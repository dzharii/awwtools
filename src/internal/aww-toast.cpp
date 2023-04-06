#include <iostream>
#include <string>
#include <vector>

#include "aww-common.hpp"
#include "internal/aww-toast.hpp"
#include "aww-banner.hpp"

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
    aww::banner::NotificationDialogBox dialogBox(
        "Aww Toast",
        message,
        aww::draw::RgbaColor(0, 0, 0, 255),
        aww::draw::RgbaColor(255, 255, 255, 255),
        5);

    // a little bit crashy, but it is fine! At least it compiles!

    dialogBox.show();
    return 0;
  }
}

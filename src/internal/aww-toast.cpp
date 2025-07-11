#include <iostream>
#include <string>
#include <vector>

#include "aww-banner.hpp"
#include "aww-common.hpp"
#include "aww-spdlog-configuration.hpp"
#include "internal/aww-toast.hpp"

namespace aww::internal::aww_toast {
int aww_toast_main(const std::vector<std::string>& cmdArgs) {
  init_default_spdlog_configuration("aww-toast");

  std::string message = "Hey!";
  if (cmdArgs.size() > 0) {
    message = aww::string::join(cmdArgs, " ");
  }
  aww::banner::NotificationDialogBox dialogBox("Aww Toast", message, aww::draw::RgbaColor(0, 0, 0, 255),
                                               aww::draw::RgbaColor(255, 255, 255, 255), 5);

  // a little bit crashy, but it is fine! At least it compiles!

  dialogBox.show();
  return 0;
}
} // namespace aww::internal::aww_toast

#include "aww-banner.hpp"
#include "aww-common.hpp"

namespace aww::banner
{
  void NotificationDialogBox::show()
  {
    aww::os::actions::showNotification(title, message);
  }
}

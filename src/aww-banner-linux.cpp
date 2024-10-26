#include "aww-banner.hpp"
#include "aww-common.hpp"

namespace aww::banner {
void NotificationDialogBox::show() { aww::os::actions::show_notification(title, message); }
} // namespace aww::banner

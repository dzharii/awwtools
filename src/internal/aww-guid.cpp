
#include <iostream>
#include <string>

#include "aww-common.hpp"
#include "internal/aww-guid.hpp"

namespace aww::internal::aww_guid {
int aww_guid_main([[maybe_unused]] const std::vector<std::string>& cmdArgs,
                  aww_guid_io_dependencies_interface& deps) {
  std::string guid;

  if (aww::Result res = deps.os_get_guid(guid); res.is_failed()) {
    std::cout << "Failed to get GUID" << res.error() << "\n";
    return 1;
  }

  if (!deps.clipboard_set_text(guid)) {
    std::cout << "Failed to copy to clipboard:\n" << guid << "\n";
    deps.show_notification("aww guid", "Failed to copy the guid to the clipboard");
    return 1;
  }

  std::cout << "Copied to clipboard:\n" << guid << "\n";
  deps.show_notification("aww guid", "The guid has been copied to the clipboard");
  return 0;
}
} // namespace aww::internal::aww_guid

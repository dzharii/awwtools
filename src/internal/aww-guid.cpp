
#include <iostream>
#include <string>

#include "clip.h"
#include "aww-common.hpp"

namespace aww::internal::aww_guid
{
  int aww_guid_main([[maybe_unused]] const std::vector<std::string> &cmdArgs)
  {
    std::string guid;
    aww::Result result = aww::util::getGuid(guid);

    if (result.isFailed())
    {
      std::cout << "Failed to get GUID" << result.error() << "\n";
      return 1;
    }

    if (clip::set_text(guid))
    {
      std::cout << "Copied to clipboard:\n"
                << guid << "\n";
      aww::os::actions::showNotification("aww guid", "The guid has been copied to the clipboard");
    }
    else
    {
      std::cout << "Failed to copy to clipboard:\n"
                << guid << "\n";
      aww::os::actions::showNotification("aww guid", "Failed to copy the guid to the clipboard");
    }
    return 0;
  }
}

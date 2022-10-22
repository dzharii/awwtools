#include "aww-common.hpp"

#include "windows.h"

namespace aww::os::actions {

    std::tuple<bool, std::string> launchFile(const std::string& path) {
       return std::make_tuple(false, "launchFile is not implemented for this Operating System :(");
    }
}

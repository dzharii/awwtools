#pragma once

#include <string>
#include <tuple>

namespace aww {

const int result = 0;
const int error = 0;

} // namespace aww


namespace aww::os::actions {
    std::tuple<bool, std::string> launchFile(const std::string&);
}

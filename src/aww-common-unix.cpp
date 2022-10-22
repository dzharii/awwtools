#include <cstdlib>
#include "aww-common.hpp"


namespace aww::os::actions {

    std::tuple<bool, std::string> launchFile(const std::string& path) {
        // check path is null
        if (path.empty()) {
            return std::make_tuple(false, "Argument path is empty");
        }
        std::string command = "xdg-open " + path;
        int result = std::system(command.c_str());
        if (result == 0) {
            return std::make_tuple(true, "");
        }
        return std::make_tuple(false, "xdg-open failed");
    }
}

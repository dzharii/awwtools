#include <iostream>
#include <string>
#include <regex>

#include "spdlog/spdlog.h"
#include "fmt/core.h"
#include "internal/aww-api.hpp"

namespace aww::internal::aww_api {

    int aww_api_main([[maybe_unused]]const std::vector<std::string> &cmdArgs, aww_api_io_dependencies_interface &deps)
    {
        spdlog::info("Hello spdlog");
        fmt::print("Hello, world from fmt PLEASE REMOVE THIS \b!\n");

        deps.show_notification("aww api", "It works", aww::call_tag("9mnoizxrhdw"));
        return 0;
    }

}

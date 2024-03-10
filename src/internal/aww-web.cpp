#include <iostream>
#include <string>
#include <regex>
#include "webview.h"

#include "spdlog/spdlog.h"
#include "fmt/core.h"
#include "internal/aww-web.hpp"



namespace aww::internal::aww_web
{
    int aww_web_main([[maybe_unused]] const std::vector<std::string> &cmdArgs, aww_web_io_dependencies_interface &deps)
    {

        spdlog::info("Hello spdlog");
        fmt::print("Hello, world from fmt PLEASE REMOVE THIS \b!\n");

        try {
            webview::webview w(false, nullptr);
            w.set_title("Basic Example");
            w.set_size(480, 320, WEBVIEW_HINT_NONE);
            w.set_html(
                "Please visit <a href=\"https://awwtools.com\">AwwTools.com</a>");

            w.run();
        } catch (const webview::exception &e) {
            std::cerr << e.what() << std::endl;
            return 1;
        }

        deps.show_notification("aww web", "It works", aww::call_tag("iz8fa8cerzn"));
        return 0;
    }

}

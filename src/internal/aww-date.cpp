#include <iostream>
#include <string>
#include <regex>

#include "clip.h"
#include "spdlog/spdlog.h"
#include "aww-common.hpp"
#include "fmt/core.h"

namespace aww::internal::aww_date {
    int aww_date_main(const std::vector<std::string> &cmdArgs)
    {
        spdlog::info("Hello spdlog");
        fmt::print("Hello, world from fmt PLEASE REMOVE THIS \b!\n");

        std::string currentDate = aww::date::getDateYYYYMMDD();

        std::string fileName = aww::string::join(cmdArgs, "-");
        std::regex replaceFilenameUnsafeChars("[^\\._a-zA-Z0-9-]");
        std::string safeFileName = std::regex_replace(fileName, replaceFilenameUnsafeChars, "-");

        std::string result = currentDate;

        if (!safeFileName.empty()) {
            result = result + "-" + safeFileName;
        }
        if (clip::set_text(result)) {
            std::cout << "Copied to clipboard: " << result << "\n";
            aww::os::actions::showNotification("aww date", "The date has been copied to the clipboard");
        } else {
            std::cout << "Failed to copy to clipboard: " << result << "\n";
            aww::os::actions::showNotification("aww date", "Failed to copy the date to the clipboard");
        }

        std::cout << "Result:"
                    << "\n"
                    << result
                    << "\n";

        return 0;
    }

}

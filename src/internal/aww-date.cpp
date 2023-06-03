#include <iostream>
#include <string>
#include <regex>


#include "spdlog/spdlog.h"
#include "fmt/core.h"
#include "internal/aww-date.hpp"

namespace aww::internal::aww_date {

    int aww_date_main(const std::vector<std::string> &cmdArgs, aww_date_io_dependencies_interface &deps)
    {
        spdlog::info("Hello spdlog");
        fmt::print("Hello, world from fmt PLEASE REMOVE THIS \b!\n");

        std::string currentDate = deps.get_date_yyyymmdd();

        std::string fileName = aww::string::join(cmdArgs, "-");
        std::regex replaceFilenameUnsafeChars("[^\\._a-zA-Z0-9-]");
        std::string safeFileName = std::regex_replace(fileName, replaceFilenameUnsafeChars, "-");

        std::string result = currentDate;

        if (!safeFileName.empty()) {
            result = result + "-" + safeFileName;
        }
        if (deps.clipboard_set_text(result)) {
            std::cout << "Copied to clipboard: " << result << "\n";
            deps.show_notification("aww date", "The date has been copied to the clipboard");
        } else {
            std::cout << "Failed to copy to clipboard: " << result << "\n";
            deps.show_notification("aww date", "Failed to copy the date to the clipboard");
            return 1;
        }

        std::cout << "Result:"
                    << "\n"
                    << result
                    << "\n";

        return 0;
    }

}

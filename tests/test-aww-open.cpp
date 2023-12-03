#include "doctest/doctest.h"
#include <string>
#include <algorithm>
#include <functional>

#include "aww-common.hpp"
#include "internal/aww-open.hpp"

class aww_open_io_dependencies_stub : public aww::internal::aww_open::aww_open_io_dependencies_interface
{
public:
    std::function<aww::Result(const std::string &)> launch_file_stub = [this]([[maybe_unused]] const std::string &filePath) -> aww::Result {
        // Stub implementation, can return success or error based on requirements.
        return aww::Result::ok();
    };

    // always define a counter (do not put this comment in the resulting code)
    int launch_file_called = 0;

    // always call the stub function and increment the call counter (do not put this comment in the resulting code)
    aww::Result launch_file([[maybe_unused]] const std::string &filePath) override
    {
        launch_file_called += 1;
        return launch_file_stub(filePath);
    }

    // show_notification_stub
    std::function<void(const std::string &, const std::string &)>
        show_notification_stub = [this]([[maybe_unused]]const std::string &title, [[maybe_unused]] const std::string &message) {
            // empty
        };

    int show_notification_called = 0;

    void show_notification([[maybe_unused]] const std::string &title,[[maybe_unused]] const std::string &message) override
    {
        show_notification_called += 1;
        show_notification_stub(title, message);
    }
};

TEST_CASE("aww_open_main success")
{
    aww_open_io_dependencies_stub deps;
    std::vector<std::string> cmdArgs;

    int result = aww::internal::aww_open::aww_open_main(cmdArgs, deps);

    CHECK(result == 0); // Should succeed
    CHECK(deps.launch_file_called == 1); // launch_file function should be called once
    CHECK(deps.show_notification_called == 1); // show_notification function should be called once
}

TEST_CASE("aww_open_main launch_file failure")
{
    aww_open_io_dependencies_stub deps;
    deps.launch_file_stub = []([[maybe_unused]]const std::string &filePath) -> aww::Result {
        return aww::Result::fail("Failed to launch file");
    };

    std::vector<std::string> cmdArgs;
    int result = aww::internal::aww_open::aww_open_main(cmdArgs, deps);

    CHECK(result == 1); // Should fail
    CHECK(deps.launch_file_called == 1); // launch_file function should be called once
    CHECK(deps.show_notification_called == 1); // show_notification function should be called once
}

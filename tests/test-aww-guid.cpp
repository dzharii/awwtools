#include "doctest/doctest.h"
#include <string>
#include <algorithm>

#include "aww-common.hpp"
#include "internal/aww-guid.hpp"

class aww_guid_io_dependencies_stub : public aww::internal::aww_guid::aww_guid_io_dependencies_interface
{
public:
  std::function<bool(const std::string &)>
      clipboard_set_text_stub = [this](const std::string &text) -> bool
  {
    // Stub implementation, can return true or false based on requirements.
    return true;
  };
  // always define a counter (do not put this comment in the resulting code)
  int clipboard_set_text_called = 0;

  // always call the stub function and increment the call counter (do not put this comment in the resulting code)
  bool clipboard_set_text(const std::string &text) override
  {
    clipboard_set_text_called += 1;
    return clipboard_set_text_stub(text);
  }

  // show_notification_stub
  std::function<void(const std::string &, const std::string &)>
      show_notification_stub = [this](const std::string &title, const std::string &message)
  {
    // empty
  };

  int show_notification_called = 0;

  void show_notification(const std::string &title, const std::string &message) override
  {
    show_notification_called += 1;
    show_notification_stub(title, message);
  }

  const std::string DEFAULT_GUID = "c7efc0ee-9dc7-463e-a708-ac614794071f";
  std::function<aww::Result(std::string &)>
      os_get_guid_stub = [this](std::string &out) -> aww::Result
  {
    out = DEFAULT_GUID;
    return aww::Result::ok();
  };
  // always define a counter (do not put this comment in the resulting code)
  int os_get_guid_called = 0;

  // always call the stub function and increment the call counter (do not put this comment in the resulting code)
  aww::Result os_get_guid(std::string &out) override
  {
    os_get_guid_called += 1;
    return os_get_guid_stub(out);
  }
};

// Test cases for aww_guid_main function
TEST_CASE("aww_guid_main success")
{
  aww_guid_io_dependencies_stub deps;
  std::vector<std::string> cmdArgs;

  int result = aww::internal::aww_guid::aww_guid_main(cmdArgs, deps);

  CHECK(result == 0);                         // Should succeed
  CHECK(deps.clipboard_set_text_called == 1); // Clipboard set text function should be called once
  CHECK(deps.show_notification_called == 1);  // Show notification function should be called once
  CHECK(deps.os_get_guid_called == 1);        // os_get_guid function should be called once
}

TEST_CASE("aww_guid_main os_get_guid failure")
{
  aww_guid_io_dependencies_stub deps;
  deps.os_get_guid_stub = [](std::string &out) -> aww::Result
  {
    return aww::Result::fail("Failed to get GUID");
  };

  std::vector<std::string> cmdArgs;
  int result = aww::internal::aww_guid::aww_guid_main(cmdArgs, deps);

  CHECK(result == 1);                         // Should fail
  CHECK(deps.clipboard_set_text_called == 0); // Clipboard set text function should not be called
  CHECK(deps.show_notification_called == 0);  // Show notification function should not be called
  CHECK(deps.os_get_guid_called == 1);        // os_get_guid function should be called once
}

TEST_CASE("aww_guid_main clipboard_set_text failure")
{
  aww_guid_io_dependencies_stub deps;
  deps.clipboard_set_text_stub = [](const std::string &text) -> bool
  {
    return false; // Simulate failure
  };

  std::vector<std::string> cmdArgs;
  int result = aww::internal::aww_guid::aww_guid_main(cmdArgs, deps);

  CHECK(result == 1);                         // Should fail
  CHECK(deps.clipboard_set_text_called == 1); // Clipboard set text function should be called once
  CHECK(deps.show_notification_called == 1);  // Show notification function should be called once
  CHECK(deps.os_get_guid_called == 1);        // os_get_guid function should be called once
}

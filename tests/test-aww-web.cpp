#include "doctest/doctest.h"
#include <string>
#include <algorithm>

#include "aww-common.hpp"
#include "internal/aww-web.hpp"

class aww_web_io_dependencies_stub : public aww::internal::aww_web::aww_web_io_dependencies_interface
{
public:
  std::function<void(const std::string &, const std::string &, aww::call_tag_t tag)>
      show_notification_stub = [this](const std::string &title, const std::string &message, aww::call_tag_t tag) {
      };

  void show_notification(const std::string &title, const std::string &message, aww::call_tag_t tag) override
  {
    show_notification_stub(title, message, tag);
  }
};

TEST_CASE("aww::internal::aww_web::aww_web_main: acceptance test")
{
  /*
  aww_web_io_dependencies_stub stubDependencies;

  int show_notification_stub_called = 0;
  std::uint64_t tag_show_notification_last_tag = 0;

  stubDependencies.show_notification_stub = [&](const std::string &title, const std::string &message, aww::call_tag_t tag)
  {
    show_notification_stub_called += 1;
    tag_show_notification_last_tag = tag.value;
  };

  std::vector<std::string> cmdArgs{};

  int mainResult = aww::internal::aww_web::aww_web_main(cmdArgs, stubDependencies);

  CHECK_MESSAGE(mainResult == 0, "Expected aww_web_main to return 0, but it returned ", mainResult);
  CHECK_MESSAGE(show_notification_stub_called == 1, "Expected #show_notification to be called 1 times, but it was called ", show_notification_stub_called, " times");

  aww::call_tag_t successTag = aww::call_tag("9mnoizxrhdw");
  CHECK_MESSAGE(
      tag_show_notification_last_tag == successTag.value,
      "Expected #tag_show_notification_last_tag=", tag_show_notification_last_tag, " to be a success tag=", successTag.value);
  */
}
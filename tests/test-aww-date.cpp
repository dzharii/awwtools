#include "doctest/doctest.h"
#include <algorithm>
#include <string>

#include "aww-common.hpp"
#include "internal/aww-date.hpp"

class aww_date_io_dependencies_stub
    : public aww::internal::aww_date::aww_date_io_dependencies_interface {
public:
  std::string get_date_yyyymmdd_returns = "2023-06-02";
  int get_date_yyyymmdd_called = 0;

  bool clipboard_set_text_returns = true;
  int clipboard_set_text_called = 0;
  std::string clipboard_set_text_last_text = "";

  std::string show_notification_last_title = "";

  std::string get_date_yyyymmdd(aww::call_tag_t tag) override {
    get_date_yyyymmdd_called += 1;
    return get_date_yyyymmdd_returns;
  }

  bool clipboard_set_text(const std::string& text, aww::call_tag_t tag) override {
    clipboard_set_text_called += 1;
    clipboard_set_text_last_text = text;
    return clipboard_set_text_returns;
  }

  std::function<void(const std::string&, const std::string&, aww::call_tag_t tag)>
      show_notification_stub =
          [this](const std::string& title, const std::string& message, aww::call_tag_t tag) {};

  void show_notification(const std::string& title, const std::string& message,
                         aww::call_tag_t tag) override {
    show_notification_stub(title, message, tag);
  }
};

TEST_CASE("aww::internal::aww_date::aww_date_main: acceptance test") {
  std::vector<std::string> cmdArgs{"this", "is", "a", "test"};
  const std::string expectedResult = "2023-06-02-this-is-a-test";

  aww_date_io_dependencies_stub stubDependencies;

  stubDependencies.get_date_yyyymmdd_returns = "2023-06-02";
  stubDependencies.clipboard_set_text_returns = true;

  int show_notification_stub_called = 0;
  std::uint64_t tag_show_notification_last_tag = 0;

  stubDependencies.show_notification_stub = [&](const std::string& title,
                                                const std::string& message, aww::call_tag_t tag) {
    show_notification_stub_called += 1;
    tag_show_notification_last_tag = tag.value;
  };

  int mainResult = aww::internal::aww_date::aww_date_main(cmdArgs, stubDependencies);

  CHECK_MESSAGE(mainResult == 0, "Expected aww_date_main to return 0, but it returned ",
                mainResult);
  CHECK_MESSAGE(stubDependencies.get_date_yyyymmdd_called == 1,
                "Expected get_date_yyyymmdd to be called once, but it was called ",
                stubDependencies.get_date_yyyymmdd_called, " times");
  CHECK_MESSAGE(stubDependencies.clipboard_set_text_called == 1,
                "Expected clipboard_set_text to be called once, but it was called ",
                stubDependencies.clipboard_set_text_called, " times");
  CHECK_MESSAGE(stubDependencies.clipboard_set_text_last_text == expectedResult,
                "Expected clipboard_set_text to be called with ", expectedResult,
                ", but it was called with ", stubDependencies.clipboard_set_text_last_text);
  CHECK_MESSAGE(show_notification_stub_called == 1,
                "Expected #show_notification to be called 1 times, but it was called ",
                show_notification_stub_called, " times");

  aww::call_tag_t successTag = aww::call_tag("tssis4p5ta2");
  CHECK_MESSAGE(tag_show_notification_last_tag == successTag.value,
                "Expected #tag_show_notification_last_tag=", tag_show_notification_last_tag,
                " to be a success tag=", successTag.value);
}

TEST_CASE("aww::internal::aww_date::aww_date_main: negative test unable to set clipboard") {
  std::vector<std::string> cmdArgs{"this", "is", "a", "test"};
  const std::string expectedResult = "2023-06-02-this-is-a-test";

  aww_date_io_dependencies_stub stubDependencies;

  stubDependencies.get_date_yyyymmdd_returns = "2023-06-02";
  stubDependencies.clipboard_set_text_returns = false;

  int show_notification_stub_called = 0;
  std::uint64_t tag_show_notification_last_tag = 0;

  stubDependencies.show_notification_stub = [&](const std::string& title,
                                                const std::string& message, aww::call_tag_t tag) {
    show_notification_stub_called += 1;
    tag_show_notification_last_tag = tag.value;
  };

  int mainResult = aww::internal::aww_date::aww_date_main(cmdArgs, stubDependencies);

  CHECK_MESSAGE(mainResult != 0, "Expected aww_date_main to return non-zero, but it returned ",
                mainResult);
  CHECK_MESSAGE(stubDependencies.get_date_yyyymmdd_called == 1,
                "Expected get_date_yyyymmdd to be called once, but it was called ",
                stubDependencies.get_date_yyyymmdd_called, " times");
  CHECK_MESSAGE(stubDependencies.clipboard_set_text_called == 1,
                "Expected clipboard_set_text to be called once, but it was called ",
                stubDependencies.clipboard_set_text_called, " times");
  CHECK_MESSAGE(stubDependencies.clipboard_set_text_last_text == expectedResult,
                "Expected clipboard_set_text to be called with ", expectedResult,
                ", but it was called with ", stubDependencies.clipboard_set_text_last_text);
  CHECK_MESSAGE(show_notification_stub_called == 1,
                "Expected #show_notification to be called 1 times, but it was called ",
                show_notification_stub_called, " times");

  aww::call_tag_t failTag = aww::call_tag("730v5jc2d3o");
  CHECK_MESSAGE(tag_show_notification_last_tag == failTag.value,
                "Expected #tag_show_notification_last_tag=", tag_show_notification_last_tag,
                " to be a fail tag=", failTag.value);
}

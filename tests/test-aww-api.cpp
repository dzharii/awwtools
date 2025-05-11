#include "doctest/doctest.h"
#include <algorithm>
#include <string>

#include "aww-common.hpp"
#include "internal/aww-api.hpp"

class aww_api_io_dependencies_stub : public aww::internal::aww_api::aww_api_io_dependencies_interface {
public:
  std::function<void(const std::string&, const std::string&)> show_notification_stub =
      [this](const std::string& title, const std::string& message) {};

  void show_notification(const std::string& title, const std::string& message) override {
    show_notification_stub(title, message);
  }
};

TEST_CASE("aww::internal::aww_api::aww_api_main: acceptance test") {
  aww_api_io_dependencies_stub stubDependencies;

  int show_notification_stub_called = 0;
  stubDependencies.show_notification_stub = [&](const std::string& title, const std::string& message) {
    show_notification_stub_called += 1;
  };

  std::vector<std::string> cmdArgs{};

  int mainResult = aww::internal::aww_api::aww_api_main(cmdArgs, stubDependencies);

  CHECK_MESSAGE(mainResult == 0, "Expected aww_api_main to return 0, but it returned ", mainResult);
  CHECK_MESSAGE(show_notification_stub_called == 1,
                "Expected #show_notification to be called 1 times, but it was called ",
                show_notification_stub_called, " times");
}
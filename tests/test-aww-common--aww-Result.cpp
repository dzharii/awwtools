#include "doctest/doctest.h"
#include "aww-common.hpp"
#include <vector>
#include <string>
#include <stdexcept>

TEST_CASE("aww::Result creates a successful result") {
  aww::Result result = aww::Result::ok();
  CHECK(result.success());
  CHECK(!result.failed());
}

TEST_CASE("aww::Result creates a failed result with error message") {
  aww::Result result = aww::Result::failed("error");
  CHECK(!result.success());
  CHECK(result.failed());
  CHECK(result.error() == "error");
}

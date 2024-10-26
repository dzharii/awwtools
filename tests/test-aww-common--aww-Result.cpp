#include "aww-common.hpp"
#include "doctest/doctest.h"
#include <stdexcept>
#include <string>
#include <vector>

TEST_CASE("aww::Result creates a successful result") {
  aww::Result result = aww::Result::ok();
  CHECK(result.is_ok());
  CHECK(!result.is_failed());
}

TEST_CASE("aww::Result creates a failed result with error message") {
  aww::Result result = aww::Result::fail("error");
  CHECK(!result.is_ok());
  CHECK(result.is_failed());
  CHECK(result.error() == "error");
}

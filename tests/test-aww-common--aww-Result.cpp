#include "doctest/doctest.h"
#include "aww-common.hpp"
#include <vector>
#include <string>
#include <stdexcept>

TEST_CASE("aww::Result creates a successful result") {
  aww::Result result = aww::Result::ok();
  CHECK(result.isOk());
  CHECK(!result.isFailed());
}

TEST_CASE("aww::Result creates a failed result with error message") {
  aww::Result result = aww::Result::fail("error");
  CHECK(!result.isOk());
  CHECK(result.isFailed());
  CHECK(result.error() == "error");
}

#include "doctest/doctest.h"
#include <algorithm>
#include <string>

#include "aww-common.hpp"
#include "internal/aww-tag.hpp"

TEST_CASE("aww::internal::aww_tag::makeTag creates a tag of the specified length = 11") {
  std::string tag = aww::internal::aww_tag::makeTag(11);
  CHECK(tag.length() == 11);
}

TEST_CASE("aww::internal::aww_tag::makeTag length = 50 has letters a..z and numbers 0..9") {
  std::string tag = aww::internal::aww_tag::makeTag(50);

  bool hasNumbers = std::any_of(tag.begin(), tag.end(), [](char c) { return std::isdigit(c); });

  bool hasLetters = std::any_of(tag.begin(), tag.end(), [](char c) { return std::isalpha(c); });

  CHECK_MESSAGE(hasNumbers, "The  tag of size 50 must include numbers: 0..9\n Actual: ", tag);
  CHECK_MESSAGE(hasLetters, "The  tag of size 50 must include letters: a..z\n Actual: ", tag);
}

TEST_CASE("aww::internal::aww_tag::makeTag creates a tag of the specified length = 0") {
  std::string tag = aww::internal::aww_tag::makeTag(0);
  CHECK(tag.length() == 0);
}

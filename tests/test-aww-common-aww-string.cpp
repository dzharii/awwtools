#include "doctest/doctest.h"
#include "aww-common.hpp"

// doctests for aww::string::join std::string join(const std::vector<std::string> &, const std::string &);
TEST_CASE("aww::string::join returns joined string")
{
  CHECK(aww::string::join({"hello", "world"}, " ") == "hello world");
  CHECK(aww::string::join({"hello", "world"}, " - ") == "hello - world");
  CHECK(aww::string::join({"hello"}, " - ") == "hello");
  CHECK(aww::string::join({ }, " - ") == "");
}

// doctests for aww::string::leftPadding std::string leftPadding(const std::string &, const char &, const size_t &);
TEST_CASE("aww::string::leftPadding returns left padded string")
{
  CHECK(aww::string::leftPadding("hello", ' ', 10) == "     hello");
  CHECK(aww::string::leftPadding("hello", ' ', 5) == "hello");
  CHECK(aww::string::leftPadding("hello", ' ', 1) == "hello");
  CHECK(aww::string::leftPadding("hello", ' ', 0) == "hello");
}

// doctests for aww::string::toupper std::string toupper(const std::string &);
TEST_CASE("aww::string::toupper returns uppercase string")
{
  CHECK(aww::string::toupper("hello") == "HELLO");
  CHECK(aww::string::toupper("HELLO") == "HELLO");
  CHECK(aww::string::toupper("hElLo") == "HELLO");
  CHECK(aww::string::toupper("HeLlO") == "HELLO");
  CHECK(aww::string::toupper("hElLo") == "HELLO");
  CHECK(aww::string::toupper("hElLo") == "HELLO");
  CHECK(aww::string::toupper("") == "");
}

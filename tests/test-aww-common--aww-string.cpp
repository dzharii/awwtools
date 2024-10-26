#include "aww-common.hpp"
#include "doctest/doctest.h"

// doctests for aww::string::join std::string join(const std::vector<std::string> &, const
// std::string &);
TEST_CASE("aww::string::join returns joined string") {
  CHECK(aww::string::join({"hello", "world"}, " ") == "hello world");
  CHECK(aww::string::join({"hello", "world"}, " - ") == "hello - world");
  CHECK(aww::string::join({"hello"}, " - ") == "hello");
  CHECK(aww::string::join({}, " - ") == "");
}

// doctests for aww::string::left_padding std::string left_padding(const std::string &, const char
// &, const size_t &);
TEST_CASE("aww::string::left_padding returns left padded string") {
  CHECK(aww::string::left_padding("hello", ' ', 10) == "     hello");
  CHECK(aww::string::left_padding("hello", ' ', 5) == "hello");
  CHECK(aww::string::left_padding("hello", ' ', 1) == "hello");
  CHECK(aww::string::left_padding("hello", ' ', 0) == "hello");
}

// doctests for aww::string::to_upper std::string to_upper(const std::string &);
TEST_CASE("aww::string::to_upper returns uppercase string") {
  CHECK(aww::string::to_upper("hello") == "HELLO");
  CHECK(aww::string::to_upper("HELLO") == "HELLO");
  CHECK(aww::string::to_upper("hElLo") == "HELLO");
  CHECK(aww::string::to_upper("HeLlO") == "HELLO");
  CHECK(aww::string::to_upper("hElLo") == "HELLO");
  CHECK(aww::string::to_upper("hElLo") == "HELLO");
  CHECK(aww::string::to_upper("") == "");
}

// doctests for aww::string::to_lower std::string to_lower(const std::string &);
TEST_CASE("aww::string::to_lower returns lowercase string") {
  CHECK(aww::string::to_lower("hello") == "hello");
  CHECK(aww::string::to_lower("HELLO") == "hello");
  CHECK(aww::string::to_lower("hElLo") == "hello");
  CHECK(aww::string::to_lower("HeLlO") == "hello");
  CHECK(aww::string::to_lower("hElLo") == "hello");
  CHECK(aww::string::to_lower("hElLo") == "hello");
  CHECK(aww::string::to_lower("") == "");
}

// doctests for aww::string::ends_with bool ends_with(const std::string& str, const std::string&
// suffix);
TEST_CASE("aww::string::ends_with returns true if string ends with suffix") {
  CHECK(aww::string::ends_with("hello", "o"));
  CHECK(aww::string::ends_with("hello", "lo"));
  CHECK(aww::string::ends_with("hello", "hello"));
  CHECK(aww::string::ends_with("hello", ""));
  CHECK(aww::string::ends_with("", ""));
  CHECK(aww::string::ends_with("", "hello") == false);
  CHECK(aww::string::ends_with("hello", "hello world") == false);
}

// doctests for aww::string::to_valid_identifier std::string to_valid_identifier(const std::string
// &);
TEST_CASE("aww::string::to_valid_identifier returns valid identifier") {
  CHECK(aww::string::to_valid_identifier("hello") == "hello");
  CHECK(aww::string::to_valid_identifier("hello123") == "hello123");
  CHECK(aww::string::to_valid_identifier("hello-123") == "hello_123");
  CHECK(aww::string::to_valid_identifier("Hello World") == "Hello_World");
  CHECK(aww::string::to_valid_identifier("hello world") == "hello_world");
  CHECK(aww::string::to_valid_identifier("hello-world") == "hello_world");
  CHECK(aww::string::to_valid_identifier("hello_world") == "hello_world");
  CHECK(aww::string::to_valid_identifier("hello/world") == "hello_world");
  CHECK(aww::string::to_valid_identifier("hello\\world") == "hello_world");
  CHECK(aww::string::to_valid_identifier("hello+world") == "hello_world");
  CHECK(aww::string::to_valid_identifier("hello=world") == "hello_world");
  CHECK(aww::string::to_valid_identifier("hello:world") == "hello_world");
  CHECK(aww::string::to_valid_identifier("hello;world") == "hello_world");
  CHECK(aww::string::to_valid_identifier("hello,world") == "hello_world");
  CHECK(aww::string::to_valid_identifier("hello.world") == "hello_world");
  CHECK(aww::string::to_valid_identifier("hello?world") == "hello_world");
  CHECK(aww::string::to_valid_identifier("hello!world") == "hello_world");
  CHECK(aww::string::to_valid_identifier("hello@world") == "hello_world");
  CHECK(aww::string::to_valid_identifier("hello#world") == "hello_world");
  CHECK(aww::string::to_valid_identifier("hello$world") == "hello_world");
  CHECK(aww::string::to_valid_identifier("hello%world") == "hello_world");
  CHECK(aww::string::to_valid_identifier("hello^world") == "hello_world");
  CHECK(aww::string::to_valid_identifier("hello&world") == "hello_world");
  CHECK(aww::string::to_valid_identifier("hello*world") == "hello_world");
  CHECK(aww::string::to_valid_identifier("hello(world") == "hello_world");
  CHECK(aww::string::to_valid_identifier("hello)world") == "hello_world");
  CHECK(aww::string::to_valid_identifier("hello(world)") == "hello_world_");
}

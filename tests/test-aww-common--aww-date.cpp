#include "doctest/doctest.h"
#include "aww-common.hpp"


TEST_CASE("aww::date::getDateYYYYMMDD returns valid date string")
{
  std::string date = aww::date::get_date_YYYYMMDD();
  CHECK_MESSAGE(date.length() == 10, "Date string should be 10 characters long YYYY-MM-DD");
  CHECK_MESSAGE(date[4] == '-', "Date string should have a dash at position 4");
  CHECK_MESSAGE(date[7] == '-', "Date string should have a dash at position 7");
}

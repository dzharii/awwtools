#include "doctest/doctest.h"
#include "aww-common.hpp"
#include <vector>
#include <string>
#include <stdexcept>

TEST_CASE("aww::Result<T> where T=bool;  creates a successful result")
{
  aww::Result<bool> result = aww::Result<bool>::ok(true);
  CHECK_MESSAGE(result.hasValue(), "Result should have a value");
  CHECK_MESSAGE(!result.hasError(), "Result should not have an error");
  CHECK_MESSAGE(result.value(), "Result value should be true");
}

TEST_CASE("aww::Result<T> where T=bool;  creates a failed result")
{
  aww::Result<bool> result = aww::Result<bool>::failed("error message");
  CHECK_MESSAGE(!result.hasValue(), "Result should not have a value");
  CHECK_MESSAGE(result.hasError(), "Result should have an error");
  CHECK_MESSAGE(result.error() == "error message", "Result error should be 'error message'");
}

TEST_CASE("aww::Result<T> where T=bool; value() throws exception on failed result")
{
  aww::Result<bool> result = aww::Result<bool>::failed("error message");
  CHECK_THROWS_AS(result.value(), std::runtime_error);
}

TEST_CASE("aww::Result<T> where T=int 42;  creates a successful result")
{
  aww::Result<int> result = aww::Result<int>::ok(42);
  CHECK_MESSAGE(result.hasValue(), "Result should have a value");
  CHECK_MESSAGE(!result.hasError(), "Result should not have an error");
  CHECK_MESSAGE(result.value() == 42, "Result value should be 42");
}

TEST_CASE("aww::Result<T> where T=int 42;  creates a failed result")
{
  aww::Result<int> result = aww::Result<int>::failed("error message");
  CHECK_MESSAGE(!result.hasValue(), "Result should not have a value");
  CHECK_MESSAGE(result.hasError(), "Result should have an error");
  CHECK_MESSAGE(result.error() == "error message", "Result error should be 'error message'");
}

TEST_CASE("aww::Result<T> where T=std::vector {1, 2, 3};  creates a successful result")
{
  std::vector<int> v = { 1, 2, 3 };
  aww::Result<std::vector<int>> result = aww::Result<std::vector<int>>::ok(v);
  CHECK_MESSAGE(result.hasValue(), "Result should have a value");
  CHECK_MESSAGE(!result.hasError(), "Result should not have an error");
  CHECK_MESSAGE(result.value() == v, "Result value should be {1, 2, 3}");

  CHECK_MESSAGE(result.value()[0] == 1, "Result value[0] should be 1");
  CHECK_MESSAGE(result.value()[1] == 2, "Result value[1] should be 2");
  CHECK_MESSAGE(result.value()[2] == 3, "Result value[2] should be 3");
}

TEST_CASE("aww::Result<T> where T=std::vector {1, 2, 3};  creates a failed result")
{
  std::vector<int> v = { 1, 2, 3 };
  aww::Result<std::vector<int>> result = aww::Result<std::vector<int>>::failed("error message");
  CHECK_MESSAGE(!result.hasValue(), "Result should not have a value");
  CHECK_MESSAGE(result.hasError(), "Result should have an error");
  CHECK_MESSAGE(result.error() == "error message", "Result error should be 'error message'");
}




#include <string>
#include <algorithm>
#include <ctime>

#include "internal/aww-tag.hpp"


// create a string of random characters a..z and 0..9 of the specified length
std::string aww::internal::aww_tag::makeTag(const size_t length)
{
  constexpr const char charset[] =
    "0123456789"
    "abcdefghijkmnopqrstuvwxyz";

  constexpr const size_t charsetLength = (sizeof(charset)) - 1;

  std::srand(static_cast<unsigned int>(std::time(nullptr)));

  std::string tag(length, ' ');

  std::generate_n(tag.begin(), length, [&]() {

    const std::size_t randomIndex = static_cast<std::size_t>(
      static_cast<unsigned int>(std::rand()) % charsetLength);

    return charset[randomIndex];

  });
  return tag;
}

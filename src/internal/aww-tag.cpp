#include <algorithm>
#include <ctime>
#include <iostream>
#include <string>

#include "aww-common.hpp"
#include "clip.h"
#include "internal/aww-tag.hpp"

namespace aww::internal::aww_tag {
int aww_tag_main(const std::vector<std::string>& cmdArgs) {
  size_t length = 11;
  int userInput = 0;
  bool useDefaultInput = cmdArgs.size() == 0;
  if (!useDefaultInput) {
    try {
      userInput = std::stoi(cmdArgs[0]);
      if (userInput < 1) {
        std::cout << "Invalid argument: '" << userInput << "', must be a positive number\n";
        return 1;
      }
      length = static_cast<size_t>(userInput);
    } catch (const std::invalid_argument&) {
      std::cout << "Invalid argument, not a number\n";
      return 1;
    } catch (const std::out_of_range&) {
      std::cout << "Invalid argument, the number is too big\n";
      return 1;
    }
  }

  std::string tag = aww::internal::aww_tag::makeTag(length);

  if (clip::set_text(tag)) {
    std::cout << "Copied to clipboard:\n" << tag << "\n";
    aww::os::actions::show_notification("aww tag", "The tag has been copied to the clipboard");
  } else {
    std::cout << "Failed to copy to clipboard:\n" << tag << "\n";
    aww::os::actions::show_notification("aww tag", "Failed to copy the tag to the clipboard");
    return 1;
  }
  return 0;
}

// create a string of random characters a..z and 0..9 of the specified length
std::string makeTag(const size_t length) {
  constexpr const char charset[] = "0123456789"
                                   "abcdefghijkmnopqrstuvwxyz";

  constexpr const size_t charsetLength = (sizeof(charset)) - 1;

  std::srand(static_cast<unsigned int>(std::time(nullptr)));

  std::string tag(length, ' ');

  std::generate_n(tag.begin(), length, [&]() {
    const std::size_t randomIndex =
        static_cast<std::size_t>(static_cast<unsigned int>(std::rand()) % charsetLength);

    return charset[randomIndex];
  });
  return tag;
}
} // namespace aww::internal::aww_tag

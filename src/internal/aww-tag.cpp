#include <algorithm>
#include <ctime>
#include <iostream>
#include <string>

#include "aww-common.hpp"
#include "aww-spdlog-configuration.hpp"
#include "clip.h"
#include "internal/aww-tag.hpp"
#include "spdlog/spdlog.h"

namespace aww::internal::aww_tag {
int aww_tag_main(const std::vector<std::string>& cmdArgs) {
  init_default_spdlog_configuration("aww-tag");
  auto mutable_cmd_args = cmdArgs;

  //  2025-03-23 no_side_effects will still log errors
  bool no_side_effects = aww::erase_flag_from_args(mutable_cmd_args, aww::constants::CMD_FLAG_NO_SIDE_EFFECTS);

  size_t length = 11;
  int user_input = 0;
  bool use_default_input = mutable_cmd_args.size() == 0;
  if (!use_default_input) {
    try {
      user_input = std::stoi(mutable_cmd_args[0]);
      if (user_input < 1) {
        spdlog::error("Invalid argument: '{}', must be a positive number\n", user_input);
        return 1;
      }
      length = static_cast<size_t>(user_input);
    } catch (const std::invalid_argument&) {
      spdlog::error("Invalid argument, not a number");
      return 1;
    } catch (const std::out_of_range&) {
      spdlog::error("Invalid argument, the number is too big");
      return 1;
    }
  }

  std::string tag = make_tag(length);
  // PRINT TAG
  std::cout << tag;

  if (clip::set_text(tag)) {
    if (!no_side_effects) {
      spdlog::info("Tag copied to clipboard");
      aww::os::actions::show_notification("aww tag", "The tag has been copied to the clipboard");
    }
  } else {
    spdlog::error("Failed to copy to clipboard:\n{}", tag);
    aww::os::actions::show_notification("aww tag", "Failed to copy the tag to the clipboard");
    return 1;
  }
  return 0;
}

// create a string of random characters a..z and 0..9 of the specified length
std::string make_tag(const size_t length) {
  constexpr const char charset[] = "0123456789"
                                   "abcdefghijkmnopqrstuvwxyz";

  constexpr const size_t charset_length = (sizeof(charset)) - 1;

  std::srand(static_cast<unsigned int>(std::time(nullptr)));

  std::string tag(length, ' ');

  std::generate_n(tag.begin(), length, [&]() {
    const std::size_t randomIndex = static_cast<std::size_t>(static_cast<unsigned int>(std::rand()) % charset_length);

    return charset[randomIndex];
  });
  return tag;
}
} // namespace aww::internal::aww_tag

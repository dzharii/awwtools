#pragma once
#ifndef AWW_TAG_HPP
#define AWW_TAG_HPP

#include <vector>
#include <string>

namespace aww::internal::aww_tag
{
  int aww_tag_main(const std::vector<std::string> &cmdArgs);
  std::string makeTag(const size_t length);
}

#endif // AWW_TAG_HPP

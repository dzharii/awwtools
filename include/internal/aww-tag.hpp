#pragma once
#ifndef AWW_TAG_HPP
#define AWW_TAG_HPP

#include <string>

namespace aww::internal::aww_tag
{
  int aww_tag_main(int argc, char **argv);
  std::string makeTag(const size_t length);
}

#endif // AWW_TAG_HPP

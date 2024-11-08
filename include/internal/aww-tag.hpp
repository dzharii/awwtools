#pragma once
#ifndef AWW_TAG_HPP
#define AWW_TAG_HPP

#include <string>
#include <vector>

namespace aww::internal::aww_tag {
int aww_tag_main(const std::vector<std::string>& cmdArgs);
std::string makeTag(const size_t length);
} // namespace aww::internal::aww_tag

#endif // AWW_TAG_HPP

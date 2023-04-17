#pragma once
#ifndef AWW_GIT_OPEN_HPP
#define AWW_GIT_OPEN_HPP

#include <filesystem>
#include <vector>
#include <string>

#include "aww-common.hpp"

namespace aww::internal::aww_git_open
{
  namespace fs = std::filesystem;

  bool findGitRepo(const fs::path &, fs::path &);
  bool tryConvertToGitUrl(const std::string &, std::string &);
  aww::Result tryFindRepositoryUrlInGitConfig(std::istream &, std::string &);
  aww::Result getRelativeUrlPath(const fs::path &, const fs::path &, std::string &);
  int aww_git_open_main(const std::vector<std::string> &cmdArgs);
}

#endif // AWW_GIT_OPEN_HPP

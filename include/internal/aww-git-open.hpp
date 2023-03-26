#pragma once
#ifndef AWW_GIT_OPEN_HPP
#define AWW_GIT_OPEN_HPP

#include <filesystem>
#include <string>

#include "aww-common.hpp"

namespace aww::internal::aww_git_open
{
  namespace fs = std::filesystem;

  bool findGitRepo(const fs::path &, fs::path &);
  bool tryConvertToGitUrl(const std::string &, std::string &);
  aww::Result tryFindRepositoryUrlInGitConfig(std::istream &, std::string &);
  aww::Result getRelativeUrlPath(const fs::path &, const fs::path &, std::string &);
  int aww_git_open_main(int argc, char **argv);
}

#endif // AWW_GIT_OPEN_HPP

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

  bool find_git_repo(const fs::path &dirPath, fs::path &gitRepoPath);
  bool try_convert_to_git_url(const std::string &inputUrl, std::string &httpUrl);
  aww::Result try_find_repository_url_in_git_config(std::istream &gitConfigStream, std::string &gitSshOrHttpUri);
  aww::Result get_relative_url_path(const fs::path &parentAbsPath, const fs::path &childAbsPath, std::string &result);
  int aww_git_open_main(const std::vector<std::string> &cmdArgs);
}

#endif // AWW_GIT_OPEN_HPP

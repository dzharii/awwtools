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

  class aww_git_open_io_dependencies_interface {
  public:
    virtual ~aww_git_open_io_dependencies_interface() = default;

    virtual aww::Result fs_get_absolute_path(fs::path &result) = 0;
    virtual aww::Result fs_get_current_directory_absolute_path(fs::path &result) = 0;
    virtual aww::Result fs_exists(const std::filesystem::path& target, bool& outFileExists) = 0;
    virtual aww::Result fs_is_directory(const std::filesystem::path &path, bool &outIsDirectory) = 0;
  };

  class aww_git_open_io_dependencies : public aww_git_open_io_dependencies_interface {
  public:

    aww::Result fs_get_absolute_path(fs::path &result) override {
      return aww::fs::get_absolute_path(result);
    }

    aww::Result fs_get_current_directory_absolute_path(fs::path &result) override {
      return aww::fs::get_current_directory_absolute_path(result);
    }

    aww::Result fs_exists(const std::filesystem::path& target, bool& outFileExists) override {
      return aww::fs::file_or_dir_exists(target, outFileExists);
    }

    aww::Result fs_is_directory(const std::filesystem::path &path, bool &outIsDirectory) {
      return aww::fs::is_directory(path, outIsDirectory);
    }
  };


  bool find_git_repo(const fs::path &dirPath, fs::path &gitRepoPath);
  bool try_convert_to_git_url(const std::string &inputUrl, std::string &httpUrl);
  aww::Result try_find_repository_url_in_git_config(std::istream &gitConfigStream, std::string &gitSshOrHttpUri);
  aww::Result get_relative_url_path(const fs::path &parentAbsPath, const fs::path &childAbsPath, std::string &result);
  int aww_git_open_main(const std::vector<std::string> &cmdArgs, aww_git_open_io_dependencies_interface &deps);
}

#endif // AWW_GIT_OPEN_HPP

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

  class aww_git_open_io_dependencies_interface
  {
  public:
    virtual ~aww_git_open_io_dependencies_interface() = default;

    /**
     * Get the absolute path of the file or directory.
     * @return The absolute path of the file or directory.
     */
    virtual inline std::filesystem::path fs_get_absolute_path(const fs::path &path) = 0;

    /**
     * Get the absolute path of the current working directory
     * @return absolute path.
     */
    virtual aww::Result fs_get_current_directory_absolute_path(fs::path &result) = 0;

    /**
     * Check if a file or directory exists at the specified path.
     * @param target The path to check.
     * @return True if a file or directory exists at the specified path, false otherwise.
     */
    virtual inline bool fs_exists(const std::filesystem::path &target) = 0;

    /**
     * Check if a path is a directory.
     * @param path The path to check.
     * @return True if the path is a directory, false otherwise.
     */
    virtual inline bool fs_is_directory(const std::filesystem::path &path) = 0;

    /**
     * Read the lines of a file into the outFileLines vector.
     * @param filePath The path of the file to read.
     * @param outFileLines Reference to a vector that will store the lines read from the file.
     * @return Aww::Result indicating the success or failure of the operation.
     */
    virtual inline aww::Result fs_read_lines(const std::filesystem::path &filePath, std::vector<std::string> &outFileLines) = 0;
  };

  class aww_git_open_io_dependencies : public aww_git_open_io_dependencies_interface
  {
  public:
    inline std::filesystem::path fs_get_absolute_path(const fs::path &path) override
    {
      return std::filesystem::absolute(path);
    }

    aww::Result fs_get_current_directory_absolute_path(fs::path &result) override
    {
      return aww::fs::get_current_directory_absolute_path(result);
    }

    inline bool fs_exists(const std::filesystem::path &target) override
    {
      return std::filesystem::exists(target);
    }

    inline bool fs_is_directory(const std::filesystem::path &path)
    {
      return std::filesystem::is_directory(path);
    }

    inline aww::Result fs_read_lines(const std::filesystem::path &filePath, std::vector<std::string> &outFileLines) override
    {
      return aww::fs::read_lines(filePath, outFileLines);
    }
  };

  bool find_git_repo(const fs::path &dirPath, fs::path &gitRepoPath, aww_git_open_io_dependencies_interface &deps);
  bool try_convert_to_git_url(const std::string &inputUrl, std::string &httpUrl);
  aww::Result try_find_repository_url_in_git_config(const std::vector<std::string>& gitConfigLines, std::string& gitSshOrHttpUri);
  aww::Result get_relative_url_path(const fs::path &parentAbsPath, const fs::path &childAbsPath, std::string &result);
  int aww_git_open_main(const std::vector<std::string> &cmdArgs, aww_git_open_io_dependencies_interface &deps);
}

#endif // AWW_GIT_OPEN_HPP

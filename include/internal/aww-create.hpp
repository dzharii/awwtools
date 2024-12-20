#pragma once
#ifndef AWW_CREATE_HPP
#define AWW_CREATE_HPP

#include <filesystem>
#include <iostream>
#include <string>
#include <vector>

#include <fstream>

#include "aww-common.hpp"

namespace aww::internal::aww_create {
namespace fs = std::filesystem;

class aww_create_io_dependencies_interface {
public:
  virtual ~aww_create_io_dependencies_interface() = default;

  /**
   * Get the current date in the format "YYYY-MM-DD".
   * @return The current date in the format "YYYY-MM-DD".
   */
  virtual std::string get_date_yyyymmdd() = 0;

  /**
   * Get the path of the current executable file.
   * @return The path of the current executable file.
   */
  virtual inline std::filesystem::path fs_get_current_executable_path() = 0;

  /**
   * Check if a file or directory exists at the specified path.
   * @param target The path to check.
   * @return True if a file or directory exists at the specified path, false otherwise.
   */
  virtual inline bool fs_exists(const std::filesystem::path& target) = 0;

  /**
   * Create directories at the specified path.
   * @param path The path at which to create directories.
   * @return True if the directories were created successfully, false otherwise.
   */
  virtual inline bool fs_create_directories(const std::filesystem::path& path) = 0;

  /**
   * Create an empty file at the specified path.
   * @param path The path at which to create the empty file.
   * @return Aww::Result indicating the success or failure of the operation.
   */
  virtual inline aww::Result fs_create_empty_file(const std::filesystem::path& path) = 0;

  /**
   * Read the lines of a file into the outFileLines vector.
   * @param filePath The path of the file to read.
   * @param outFileLines Reference to a vector that will store the lines read from the file.
   * @return Aww::Result indicating the success or failure of the operation.
   */
  virtual inline aww::Result fs_read_lines(const std::filesystem::path& filePath,
                                           std::vector<std::string>& outFileLines) = 0;

  /**
   * Write the lines from the input vector to a file specified by the filePath.
   * @param filePath The path of the file to write.
   * @param lines The vector of lines to write to the file.
   * @return Aww::Result indicating the success or failure of the operation.
   */
  virtual inline aww::Result fs_write_lines(const std::filesystem::path& filePath,
                                            const std::vector<std::string>& lines) = 0;

  /**
   * Show a notification with the specified title and message.
   * @param title The title of the notification.
   * @param message The message of the notification.
   */
  virtual void show_notification(const std::string& title, const std::string& message) = 0;
};

class aww_create_io_dependencies : public aww_create_io_dependencies_interface {
public:
  std::string get_date_yyyymmdd() override { return aww::date::get_date_YYYYMMDD(); }

  inline std::filesystem::path fs_get_current_executable_path() override {
    return aww::fs::get_current_executable_path();
  }

  inline bool fs_exists(const std::filesystem::path& target) override {
    return std::filesystem::exists(target);
  }

  inline bool fs_create_directories(const std::filesystem::path& path) override {
    return std::filesystem::create_directories(path);
  }

  // 2023-06-08
  inline aww::Result fs_create_empty_file(const std::filesystem::path& path) override {
    return aww::fs::create_empty_file(path);
  }

  inline aww::Result fs_read_lines(const std::filesystem::path& filePath,
                                   std::vector<std::string>& outFileLines) override {
    return aww::fs::read_lines(filePath, outFileLines);
  }

  inline aww::Result fs_write_lines(const std::filesystem::path& filePath,
                                    const std::vector<std::string>& lines) override {
    return aww::fs::write_lines(filePath, lines);
  }

  void show_notification(const std::string& title, const std::string& message) override {
    aww::os::actions::show_notification(title, message);
  }
};

aww::Result assume_template_path(const std::filesystem::path& awwCreateTemplatesDir,
                                 const std::filesystem::path& targetFilePath,
                                 const std::string& templateModifier,
                                 std::filesystem::path& outTemplatePath,
                                 aww_create_io_dependencies_interface& deps);
aww::Result try_create_file_by_path(const fs::path& filePath,
                                    aww_create_io_dependencies_interface& deps);
aww::Result create_new_directory_scenario(const std::filesystem::path& filePath,
                                          aww_create_io_dependencies_interface& deps);
aww::Result
append_template_content_to_new_file_scenario(const std::filesystem::path& templatePath,
                                             const std::filesystem::path& filePath,
                                             aww_create_io_dependencies_interface& deps);
int aww_create_main(const std::vector<std::string>& cmdArgs,
                    aww_create_io_dependencies_interface& deps);
} // namespace aww::internal::aww_create

#endif // AWW_CREATE_HPP

#pragma once
#ifndef AWW_CREATE_HPP
#define AWW_CREATE_HPP

#include <vector>
#include <string>
#include <filesystem>
#include <iostream>

#include <fstream>

#include "aww-common.hpp"

namespace aww::internal::aww_create
{
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
       * @param target The path to check for existence.
       * @param outFileExists Reference to a boolean variable that will be set to true if the target exists, false otherwise.
       * @return Aww::Result indicating the success or failure of the operation.
       */
      virtual inline aww::Result fs_exists(const std::filesystem::path& target, bool& outFileExists) = 0;

      /**
       * Create directories at the specified path.
       * @param path The path at which to create directories.
       * @return Aww::Result indicating the success or failure of the operation.
       */
      virtual inline aww::Result fs_create_directories(const std::filesystem::path& path) = 0;

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
      virtual inline aww::Result fs_read_lines(const std::filesystem::path& filePath, std::vector<std::string>& outFileLines) = 0;

      /**
       * Write the lines from the input vector to a file specified by the filePath.
       * @param filePath The path of the file to write.
       * @param lines The vector of lines to write to the file.
       * @return Aww::Result indicating the success or failure of the operation.
       */
      virtual inline aww::Result fs_write_lines(const std::filesystem::path& filePath, const std::vector<std::string>& lines) = 0;

      /**
       * Show a notification with the specified title and message.
       * @param title The title of the notification.
       * @param message The message of the notification.
       */
      virtual void show_notification(const std::string &title, const std::string &message) = 0;
  };

  class aww_create_io_dependencies : public aww_create_io_dependencies_interface {
    public:
      std::string get_date_yyyymmdd() override {
        return aww::date::getDateYYYYMMDD();
      }

      inline std::filesystem::path fs_get_current_executable_path() override {
        return aww::fs::getCurrentExecutablePath();
      }

      inline aww::Result fs_exists(const std::filesystem::path& target, bool& outFileExists) override {
        try {
          outFileExists = std::filesystem::exists(target);
          return aww::Result::ok();
        } catch (const std::exception& e) {
          std::string errorMessage = "Error checking existence: " + std::string(e.what());
          return aww::Result::fail(errorMessage);
        } catch (...) {
          std::string errorMessage = "Unknown error occurred while checking existence.";
          return aww::Result::fail(errorMessage);
        }
      }

      inline aww::Result fs_create_directories(const std::filesystem::path& path) {
        try {
          std::filesystem::create_directories(path);
          return aww::Result::ok();
        } catch (const std::exception& e) {
          std::string errorMessage = "Error creating directories: " + std::string(e.what());
          return aww::Result::fail(errorMessage);
        } catch (...) {
          std::string errorMessage = "Unknown error occurred while creating directories.";
          return aww::Result::fail(errorMessage);
        }
      }

      inline aww::Result fs_create_empty_file(const std::filesystem::path& path) override {
        try {
          std::ofstream file(path);
          file.close();
          return aww::Result::ok();
        } catch (const std::exception& e) {
          std::string errorMessage = "Error creating file '" + path.filename().string() + "': " + e.what();
          return aww::Result::fail(errorMessage);
        } catch (...) {
          std::string errorMessage = "Unknown error occurred while creating file '" + path.filename().string() + "'.";
          return aww::Result::fail(errorMessage);
        }
      }

      inline aww::Result fs_read_lines(const std::filesystem::path& filePath, std::vector<std::string>& outFileLines) override {
        try {
          std::ifstream file(filePath);
          if (!file.is_open()) {
            return aww::Result::fail("Failed to open file for reading.");
          }

          std::string line;
          while (std::getline(file, line)) {
            outFileLines.push_back(line);
          }

          file.close();
          return aww::Result::ok();
        } catch (const std::exception& e) {
          std::string errorMessage = "Error reading file: " + std::string(e.what());
          return aww::Result::fail(errorMessage);
        } catch (...) {
          std::string errorMessage = "Unknown error occurred while reading file.";
          return aww::Result::fail(errorMessage);
        }
      }

      inline aww::Result fs_write_lines(const std::filesystem::path& filePath, const std::vector<std::string>& lines) override {
        try {
          std::ofstream file(filePath);
          if (!file.is_open()) {
            return aww::Result::fail("Failed to open file for writing.");
          }

          for (const auto& line : lines) {
            file << line << std::endl;
          }

          file.close();
          return aww::Result::ok();
        } catch (const std::exception& e) {
          std::string errorMessage = "Error writing file: " + std::string(e.what());
          return aww::Result::fail(errorMessage);
        } catch (...) {
          std::string errorMessage = "Unknown error occurred while writing file.";
          return aww::Result::fail(errorMessage);
        }
      }

      void show_notification(const std::string &title, const std::string& message) override {
        aww::os::actions::showNotification(title, message);
      }
  };

  aww::Result assume_template_path(const std::filesystem::path& awwCreateTemplatesDir, const std::filesystem::path& targetFilePath, const std::string& templateModifier, std::filesystem::path& outTemplatePath, aww_create_io_dependencies_interface &deps);
  aww::Result try_create_file_by_path(const fs::path& filePath, aww_create_io_dependencies_interface& deps);
  aww::Result create_new_directory_scenario(const std::filesystem::path& filePath, aww_create_io_dependencies_interface &deps);
  aww::Result append_template_content_to_new_file_scenario(const std::filesystem::path& templatePath, const std::filesystem::path& filePath, aww_create_io_dependencies_interface &deps);
  int aww_create_main(const std::vector<std::string>& cmdArgs, aww_create_io_dependencies_interface& deps);
}

#endif // AWW_CREATE_HPP

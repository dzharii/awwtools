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

      virtual std::string get_date_yyyymmdd() = 0;

      virtual inline std::filesystem::path fs_get_current_executable_path() = 0;
      virtual inline aww::Result fs_exists(const std::filesystem::path &target) = 0;
      virtual inline aww::Result fs_create_directories(const std::filesystem::path& path) = 0;
      virtual inline aww::Result fs_create_empty_file(const std::filesystem::path& path) = 0;

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

      inline aww::Result fs_exists(const std::filesystem::path &target) override {

        // TODO 2023-06-03: move it to aww::fs::exists()
        if constexpr (aww::os::OSPlatform == aww::os::Platform::Windows) {

          const std::string reservedNames[] = {
            "CON",
            "PRN",
            "AUX",
            "NUL",
            "COM1",
            "COM2",
            "COM3",
            "COM4",
            "COM5",
            "COM6",
            "COM7",
            "COM8",
            "COM9",
            "LPT1",
            "LPT2",
            "LPT3",
            "LPT4",
            "LPT5",
            "LPT6",
            "LPT7",
            "LPT8",
            "LPT9" };

          std::string fileOrDirName = target.filename().string();
          fileOrDirName = aww::string::toupper(fileOrDirName);

          bool directoryNameIsReserved = std::find(
            std::begin(reservedNames),
            std::end(reservedNames),
            fileOrDirName) != std::end(reservedNames);

          if (directoryNameIsReserved) {
            std::string errorMessage = "Error checking if file or directory exists: The name is a reserved device name on Windows.";
            return aww::Result::fail(errorMessage);
          }
        }

        if (std::filesystem::exists(target)) {
          return aww::Result::ok();
        }

        return aww::Result::fail("The file or directory does not exist.");
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

      void show_notification(const std::string &title, const std::string &message) override {
        aww::os::actions::showNotification(title, message);
      }
  };

  aww::Result try_create_file_by_path(const fs::path &filePath, aww_create_io_dependencies_interface &deps);
  int aww_create_main(const std::vector<std::string> &cmdArgs, aww_create_io_dependencies_interface &deps);
}

#endif // AWW_CREATE_HPP

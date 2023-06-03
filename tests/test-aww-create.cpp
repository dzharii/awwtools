#include "doctest/doctest.h"
#include <string>
#include <vector>
#include <filesystem>

#include "aww-common.hpp"
#include "internal/aww-create.hpp"

class aww_create_io_dependencies_stub : public aww::internal::aww_create::aww_create_io_dependencies_interface {
public:
  std::string get_date_yyyymmdd_returns = "2023-06-02";
  int get_date_yyyymmdd_called = 0;

  bool fs_exists_returns = true;
  int fs_exists_called = 0;
  bool fs_exists_outExists = false;

  aww::Result fs_create_directories_returns = aww::Result::ok();
  int fs_create_directories_called = 0;

  aww::Result fs_create_empty_file_returns = aww::Result::ok();
  int fs_create_empty_file_called = 0;

  aww::Result fs_read_lines_returns = aww::Result::ok();
  int fs_read_lines_called = 0;
  std::vector<std::string> fs_read_lines_outLines;

  aww::Result fs_write_lines_returns = aww::Result::ok();
  int fs_write_lines_called = 0;
  std::vector<std::string> fs_write_lines_inLines;

  std::string show_notification_last_title = "";
  std::string show_notification_last_message = "";
  int show_notification_called = 0;

  std::string get_date_yyyymmdd() override {
    get_date_yyyymmdd_called += 1;
    return get_date_yyyymmdd_returns;
  }

  inline std::filesystem::path fs_get_current_executable_path() override {
    return std::filesystem::path();
  }

  inline aww::Result fs_exists(const std::filesystem::path& target, bool& outFileExists) override {
    fs_exists_called += 1;
    outFileExists = fs_exists_outExists;
    return aww::Result::ok();
  }

  inline aww::Result fs_create_directories(const std::filesystem::path& path) override {
    fs_create_directories_called += 1;
    return fs_create_directories_returns;
  }

  inline aww::Result fs_create_empty_file(const std::filesystem::path& path) override {
    fs_create_empty_file_called += 1;
    return fs_create_empty_file_returns;
  }

  inline aww::Result fs_read_lines(const std::filesystem::path& filePath, std::vector<std::string>& outFileLines) override {
    fs_read_lines_called += 1;
    outFileLines = fs_read_lines_outLines;
    return fs_read_lines_returns;
  }

  inline aww::Result fs_write_lines(const std::filesystem::path& filePath, const std::vector<std::string>& lines) override {
    fs_write_lines_called += 1;
    fs_write_lines_inLines = lines;
    return fs_write_lines_returns;
  }

  void show_notification(const std::string& title, const std::string& message) override {
    show_notification_called += 1;
    show_notification_last_title = title;
    show_notification_last_message = message;
  }
};

TEST_CASE("aww::internal::aww_create::aww_create_main: create directory") {
  // Test case setup
  std::string directoryPath = "test_directory";
  std::vector<std::string> cmdArgs = {directoryPath};
  aww_create_io_dependencies_stub stubDependencies;

  // Execute the function
  int mainResult = aww::internal::aww_create::aww_create_main(cmdArgs, stubDependencies);

  // Assertions
  CHECK_MESSAGE(mainResult == 0, "Expected aww_create_main to return 0, but it returned " << mainResult);

  bool directoryExists = false;
  aww::Result fsExistsResult = stubDependencies.fs_exists(directoryPath, directoryExists);
  CHECK_MESSAGE(fsExistsResult.isOk(), "Failed to check if directory exists: " << fsExistsResult.error());
  CHECK_MESSAGE(directoryExists, "Expected directory to be created, but it doesn't exist");

  // Cleanup
  std::filesystem::remove(directoryPath);
}

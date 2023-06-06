#include "doctest/doctest.h"
#include <string>
#include <vector>
#include <filesystem>

#include "aww-common.hpp"
#include "internal/aww-create.hpp"

class aww_create_io_dependencies_stub : public aww::internal::aww_create::aww_create_io_dependencies_interface {
public:
  std::function<std::string(int)> get_date_yyyymmdd_stub = [this](int count) {
    return "2023-06-02";
  };
  int get_date_yyyymmdd_called = 0;

  std::function<std::filesystem::path()> fs_get_current_executable_path_stub = [this]() {
    return std::filesystem::path();
  };
  int fs_get_current_executable_path_called = 0;

  std::function<aww::Result(int, const std::filesystem::path&, bool&)> fs_exists_stub = [this](int callCount, const std::filesystem::path& target, bool& outFileExists) -> aww::Result {
    outFileExists = fs_exists_outExists;
    return aww::Result::ok();
  };
  int fs_exists_called = 0;

  std::function<aww::Result(int, const std::filesystem::path&)> fs_create_directories_stub = [this](int callCount, const std::filesystem::path& path) -> aww::Result {
    return aww::Result::ok();
  };
  int fs_create_directories_called = 0;

  std::function<aww::Result(int, const std::filesystem::path&)> fs_create_empty_file_stub = [this](int callCount, const std::filesystem::path& path) -> aww::Result {
    return aww::Result::ok();
  };
  int fs_create_empty_file_called = 0;

  std::function<aww::Result(int, const std::filesystem::path&, std::vector<std::string>&)> fs_read_lines_stub = [this](int callCount, const std::filesystem::path& filePath, std::vector<std::string>& outFileLines) -> aww::Result {
    return aww::Result::ok();
  };
  int fs_read_lines_called = 0;

  std::function<aww::Result(int, const std::filesystem::path&, const std::vector<std::string>&)> fs_write_lines_stub = [this](int callCount, const std::filesystem::path& filePath, const std::vector<std::string>& lines) -> aww::Result {
    fs_write_lines_inLines = lines;
    return aww::Result::ok();
  };
  int fs_write_lines_called = 0;

  std::function<void(const std::string&, const std::string&)> show_notification_stub = [this](const std::string& title, const std::string& message) {

  };

  int show_notification_called = 0;

  std::string get_date_yyyymmdd() override {
    get_date_yyyymmdd_called += 1;
    return get_date_yyyymmdd_stub(get_date_yyyymmdd_called);
  }

  std::filesystem::path fs_get_current_executable_path() override {
    fs_get_current_executable_path_called += 1;
    return fs_get_current_executable_path_stub();
  }

  aww::Result fs_exists(const std::filesystem::path& target, bool& outFileExists) override {
    fs_exists_called += 1;
    return fs_exists_stub(fs_exists_called, target, outFileExists);
  }

  aww::Result fs_create_directories(const std::filesystem::path& path) override {
    fs_create_directories_called += 1;
    return fs_create_directories_stub(fs_create_directories_called, path);
  }

  aww::Result fs_create_empty_file(const std::filesystem::path& path) override {
    fs_create_empty_file_called += 1;
    return fs_create_empty_file_stub(fs_create_empty_file_called, path);
  }

  aww::Result fs_read_lines(const std::filesystem::path& filePath, std::vector<std::string>& outFileLines) override {
    fs_read_lines_called += 1;
    return fs_read_lines_stub(fs_read_lines_called, filePath, outFileLines);
  }

  aww::Result fs_write_lines(const std::filesystem::path& filePath, const std::vector<std::string>& lines) override {
    fs_write_lines_called += 1;
    return fs_write_lines_stub(fs_write_lines_called, filePath, lines);
  }

  void show_notification(const std::string& title, const std::string& message) override {
    show_notification_called += 1;
    show_notification_stub(title, message);
  }
};


TEST_CASE("aww::internal::aww_create::aww_create_main: create directory") {

}

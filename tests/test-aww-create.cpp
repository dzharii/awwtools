#include "doctest/doctest.h"
#include <filesystem>
#include <string>
#include <vector>

#include "aww-common.hpp"
#include "internal/aww-create.hpp"

class aww_create_io_dependencies_stub
    : public aww::internal::aww_create::aww_create_io_dependencies_interface {
public:
  std::function<std::string(int)> get_date_yyyymmdd_stub = [this](int count) {
    return "2023-06-02";
  };
  int get_date_yyyymmdd_called = 0;

  std::function<std::filesystem::path()> fs_get_current_executable_path_stub = [this]() {
    return std::filesystem::path();
  };
  int fs_get_current_executable_path_called = 0;

  std::function<bool(int, const std::filesystem::path&)> fs_exists_stub =
      [this](int callCount, const std::filesystem::path& target) -> bool { return true; };
  int fs_exists_called = 0;

  std::function<bool(int, const std::filesystem::path&)> fs_create_directories_stub =
      [this](int callCount, const std::filesystem::path& path) -> bool { return true; };
  int fs_create_directories_called = 0;

  std::function<aww::Result(int, const std::filesystem::path&)> fs_create_empty_file_stub =
      [this](int callCount, const std::filesystem::path& path) -> aww::Result {
    return aww::Result::ok();
  };
  int fs_create_empty_file_called = 0;

  std::function<aww::Result(int, const std::filesystem::path&, std::vector<std::string>&)>
      fs_read_lines_stub = [this](int callCount, const std::filesystem::path& filePath,
                                  std::vector<std::string>& outFileLines) -> aww::Result {
    return aww::Result::ok();
  };
  int fs_read_lines_called = 0;

  std::function<aww::Result(int, const std::filesystem::path&, const std::vector<std::string>&)>
      fs_write_lines_stub = [this](int callCount, const std::filesystem::path& filePath,
                                   const std::vector<std::string>& lines) -> aww::Result {
    return aww::Result::ok();
  };
  int fs_write_lines_called = 0;

  std::function<void(int, const std::string&, const std::string&)> show_notification_stub =
      [this](int callCount, const std::string& title, const std::string& message) {

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

  inline bool fs_exists(const std::filesystem::path& target) override {
    fs_exists_called += 1;
    return fs_exists_stub(fs_exists_called, target);
  }

  bool fs_create_directories(const std::filesystem::path& path) override {
    fs_create_directories_called += 1;
    return fs_create_directories_stub(fs_create_directories_called, path);
  }

  aww::Result fs_create_empty_file(const std::filesystem::path& path) override {
    fs_create_empty_file_called += 1;
    return fs_create_empty_file_stub(fs_create_empty_file_called, path);
  }

  aww::Result fs_read_lines(const std::filesystem::path& filePath,
                            std::vector<std::string>& outFileLines) override {
    fs_read_lines_called += 1;
    return fs_read_lines_stub(fs_read_lines_called, filePath, outFileLines);
  }

  aww::Result fs_write_lines(const std::filesystem::path& filePath,
                             const std::vector<std::string>& lines) override {
    fs_write_lines_called += 1;
    return fs_write_lines_stub(fs_write_lines_called, filePath, lines);
  }

  void show_notification(const std::string& title, const std::string& message) override {
    show_notification_called += 1;
    show_notification_stub(show_notification_called, title, message);
  }
};

TEST_CASE("aww::internal::aww_create::assume_template_path") {

  aww_create_io_dependencies_stub ioDependencies;
  const std::filesystem::path awwCreateTemplatesDir = "template-dir";
  const std::filesystem::path targetFilePath = "/tmp/target-file.md";
  std::string templateModifier = "hello";

  std::filesystem::path outTemplatePath;

  SUBCASE("Valid template path with modifier exists") {
    // ARRANGE
    ioDependencies.fs_exists_stub = [&](int callCount,
                                        const std::filesystem::path& target) -> bool {
      // check if template file exists
      if (target.filename() == "template-hello.md") {
        return true;
      }
      return false;
    };

    // ACT
    aww::Result result = assume_template_path(awwCreateTemplatesDir, targetFilePath,
                                              templateModifier, outTemplatePath, ioDependencies);

    // ASSERT
    std::filesystem::path outTemplatePathExpected = awwCreateTemplatesDir / "template-hello.md";
    CHECK_MESSAGE(outTemplatePath == outTemplatePathExpected,
                  "outTemplatePath should be '" << outTemplatePathExpected << "' but was '"
                                                << outTemplatePath << "'");
  }

  SUBCASE("template path with modifier does not exist") {
    // ARRANGE
    ioDependencies.fs_exists_stub =
        [&](int callCount, const std::filesystem::path& target) -> bool { return false; };

    // ACT
    aww::Result result = assume_template_path(awwCreateTemplatesDir, targetFilePath,
                                              templateModifier, outTemplatePath, ioDependencies);

    // ASSERT
    std::filesystem::path outTemplatePathExpected = awwCreateTemplatesDir / "template.md";
    CHECK_MESSAGE(outTemplatePath == outTemplatePathExpected,
                  "outTemplatePath should be '" << outTemplatePathExpected << "' but was '"
                                                << outTemplatePath << "'");
  }
  SUBCASE("Valid template path without modifier") {
    templateModifier = "";
    // ARRANGE
    ioDependencies.fs_exists_stub =
        [&](int callCount, const std::filesystem::path& target) -> bool { return false; };

    // ACT
    aww::Result result = assume_template_path(awwCreateTemplatesDir, targetFilePath,
                                              templateModifier, outTemplatePath, ioDependencies);

    // ASSERT
    std::filesystem::path outTemplatePathExpected = awwCreateTemplatesDir / "template.md";
    CHECK_MESSAGE(outTemplatePath == outTemplatePathExpected,
                  "outTemplatePath should be '" << outTemplatePathExpected << "' but was '"
                                                << outTemplatePath << "'");
  }
}

TEST_CASE("aww::internal::aww_create::try_create_file_by_path") {
  aww_create_io_dependencies_stub ioDependencies;
  std::filesystem::path filePath = "/tmp/newfile.txt";

  SUBCASE("File already exists") {
    // ARRANGE
    ioDependencies.fs_exists_stub =
        [&](int callCount, const std::filesystem::path& target) -> bool { return true; };

    // ACT
    aww::Result result = try_create_file_by_path(filePath, ioDependencies);

    // ASSERT
    CHECK_MESSAGE(result.is_failed(), "Result should be failed because file already exists");
  }

  SUBCASE("Invalid path") {
    // ARRANGE
    ioDependencies.fs_exists_stub =
        [&](int callCount, const std::filesystem::path& target) -> bool { return false; };

    // ACT
    aww::Result result = try_create_file_by_path("", ioDependencies);

    // ASSERT
    CHECK_MESSAGE(result.is_failed(), "Result should be failed because path is invalid");
  }

  SUBCASE("Create file in existing directory") {
    // ARRANGE
    ioDependencies.fs_exists_stub =
        [&](int callCount, const std::filesystem::path& target) -> bool { return false; };

    ioDependencies.fs_create_empty_file_stub =
        [&](int callCount, const std::filesystem::path& path) -> aww::Result {
      return aww::Result::ok();
    };

    // ACT
    aww::Result result = try_create_file_by_path(filePath, ioDependencies);

    // ASSERT
    CHECK_MESSAGE(result.is_ok(), "Result should be successful");
  }

  SUBCASE("Create file with nested directories") {
    // ARRANGE
    ioDependencies.fs_exists_stub =
        [&](int callCount, const std::filesystem::path& target) -> bool { return false; };

    ioDependencies.fs_create_directories_stub =
        [&](int callCount, const std::filesystem::path& path) -> bool { return true; };

    ioDependencies.fs_create_empty_file_stub =
        [&](int callCount, const std::filesystem::path& path) -> aww::Result {
      return aww::Result::ok();
    };

    // ACT
    aww::Result result =
        try_create_file_by_path("/tmp/nested/directory/newfile.txt", ioDependencies);

    // ASSERT
    CHECK_MESSAGE(result.is_ok(), "Result should be successful");
  }
}

TEST_CASE("aww::internal::aww_create::create_new_directory_scenario") {
  aww_create_io_dependencies_stub ioDependencies;
  std::filesystem::path filePath = "/tmp/new_directory";

  SUBCASE("Directory already exists") {
    // ARRANGE
    ioDependencies.fs_exists_stub =
        [&](int callCount, const std::filesystem::path& target) -> bool { return false; };

    // ACT
    aww::Result result = create_new_directory_scenario(filePath, ioDependencies);

    // ASSERT
    CHECK_MESSAGE(result.is_ok(), "Result should be successful even if directory already exists");
  }

  SUBCASE("Create directory") {
    // ARRANGE
    ioDependencies.fs_exists_stub =
        [&](int callCount, const std::filesystem::path& target) -> bool { return false; };

    ioDependencies.fs_create_directories_stub =
        [&](int callCount, const std::filesystem::path& path) -> bool { return true; };

    // ACT
    aww::Result result = create_new_directory_scenario(filePath, ioDependencies);

    // ASSERT
    CHECK_MESSAGE(result.is_ok(), "Result should be successful");
  }

  SUBCASE("Failed to create directory") {
    // ARRANGE
    ioDependencies.fs_exists_stub =
        [&](int callCount, const std::filesystem::path& target) -> bool { return false; };

    ioDependencies.fs_create_directories_stub =
        [&](int callCount, const std::filesystem::path& path) -> bool { return false; };

    // ACT
    aww::Result result = create_new_directory_scenario(filePath, ioDependencies);

    // ASSERT
    CHECK_MESSAGE(result.is_failed(), "Result should be failed if failed to create directory");
  }
}

TEST_CASE("aww::internal::aww_create::append_template_content_to_new_file_scenario") {
  aww_create_io_dependencies_stub ioDependencies;
  std::filesystem::path templatePath = "/path/to/template.txt";
  std::filesystem::path filePath = "/path/to/new_file.txt";

  SUBCASE("Failed to read template file") {
    // ARRANGE
    ioDependencies.fs_read_lines_stub = [&](int callCount, const std::filesystem::path& filePath,
                                            std::vector<std::string>& outFileLines) -> aww::Result {
      return aww::Result::fail("Failed to read template file");
    };

    // ACT
    aww::Result result =
        append_template_content_to_new_file_scenario(templatePath, filePath, ioDependencies);

    // ASSERT
    CHECK_MESSAGE(result.is_failed(), "Result should be failed if failed to read template file");
  }

  SUBCASE("Successful template content append - processing") {
    // ARRANGE
    std::vector<std::string> newFileLines;
    std::vector<std::string> templateLines = {"Existing Line 1", "___FILE_NAME___",
                                              "___CAPITALIZED_FILE_NAME___"};
    std::vector<std::string> expectedLines = {"Existing Line 1", "new_file", "NEW_FILE"};

    ioDependencies.fs_read_lines_stub = [&](int callCount, const std::filesystem::path& filePath,
                                            std::vector<std::string>& outFileLines) -> aww::Result {
      outFileLines = templateLines;
      return aww::Result::ok();
    };

    ioDependencies.fs_write_lines_stub = [&](int callCount, const std::filesystem::path& filePath,
                                             const std::vector<std::string>& lines) -> aww::Result {
      newFileLines = lines;
      return aww::Result::ok();
    };

    // ACT
    aww::Result result =
        append_template_content_to_new_file_scenario(templatePath, filePath, ioDependencies);

    // ASSERT
    CHECK_MESSAGE(result.is_ok(), "Result should be successful");

    CHECK_MESSAGE(newFileLines == expectedLines, "New file lines should match the expected lines. "
                                                     << "\nExpected:\n"
                                                     << aww::string::join(expectedLines, "\n")
                                                     << "\nActual:\n"
                                                     << aww::string::join(newFileLines, "\n"));
  }

  SUBCASE("Failed to write file") {
    // ARRANGE
    std::vector<std::string> templateLines = {"Line 1", "Line 2", "Line 3"};

    ioDependencies.fs_read_lines_stub = [&](int callCount, const std::filesystem::path& filePath,
                                            std::vector<std::string>& outFileLines) -> aww::Result {
      outFileLines = templateLines;
      return aww::Result::ok();
    };

    ioDependencies.fs_write_lines_stub = [&](int callCount, const std::filesystem::path& filePath,
                                             const std::vector<std::string>& lines) -> aww::Result {
      return aww::Result::fail("Failed to write file");
    };

    // ACT
    aww::Result result =
        append_template_content_to_new_file_scenario(templatePath, filePath, ioDependencies);

    // ASSERT
    CHECK_MESSAGE(result.is_failed(), "Result should be failed if failed to write file");
  }
}
# MDTREE (`output-aww-create.md`)


- $Path = `.`
- $FilterPath = ``
- $FilterName = ``
- $Include = `*aww-create*.cpp,*aww-create*.hpp`
- $Output = `output-aww-create.md`


Generated on `2023-07-29 13:21:14`

[TOC]

## File content `app\aww-create.cpp`:

```cpp
#include <iostream>
#include <vector>
#include "internal/aww-create.hpp"

int main(int argc, char **argv)
{
  try
  {
    std::vector<std::string> cmdArgs(argv, argv + argc);
    cmdArgs.erase(cmdArgs.begin()); // remove first element

    aww::internal::aww_create::aww_create_io_dependencies deps;
    return aww::internal::aww_create::aww_create_main(cmdArgs, deps);
  }
  catch (std::exception &ex)
  {
    std::cerr << ex.what() << "\n";
    return 1;
  }
  catch (...)
  {
    std::cerr << "Caught unknown exception.\n";
    return 1;
  }
}

```

## File content `include\internal\aww-create.hpp`:

```hpp
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
        return aww::date::get_date_YYYYMMDD();
      }

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

      inline aww::Result fs_read_lines(const std::filesystem::path& filePath, std::vector<std::string>& outFileLines) override {
        return aww::fs::read_lines(filePath, outFileLines);
      }

      inline aww::Result fs_write_lines(const std::filesystem::path& filePath, const std::vector<std::string>& lines) override {
        return aww::fs::write_lines(filePath, lines);
      }

      void show_notification(const std::string &title, const std::string& message) override {
        aww::os::actions::show_notification(title, message);
      }
  };

  aww::Result assume_template_path(const std::filesystem::path& awwCreateTemplatesDir, const std::filesystem::path& targetFilePath, const std::string& templateModifier, std::filesystem::path& outTemplatePath, aww_create_io_dependencies_interface &deps);
  aww::Result try_create_file_by_path(const fs::path& filePath, aww_create_io_dependencies_interface& deps);
  aww::Result create_new_directory_scenario(const std::filesystem::path& filePath, aww_create_io_dependencies_interface &deps);
  aww::Result append_template_content_to_new_file_scenario(const std::filesystem::path& templatePath, const std::filesystem::path& filePath, aww_create_io_dependencies_interface &deps);
  int aww_create_main(const std::vector<std::string>& cmdArgs, aww_create_io_dependencies_interface& deps);
}

#endif // AWW_CREATE_HPP

```

## File content `src\internal\aww-create.cpp`:

```cpp
#include <iostream>
#include <fstream>

#include <string>
#include <vector>
#include <filesystem>
#include <algorithm>
#include <cctype>

#include "aww-common.hpp"
#include "aww-inspiration.hpp"

#include "internal/aww-create.hpp"

namespace aww::internal::aww_create
{
  namespace fs = std::filesystem;

  int aww_create_main(const std::vector<std::string> &cmdArgs, aww_create_io_dependencies_interface &deps)
  {
    if (cmdArgs.size() == 0)
    {
      std::cout << "Usage: aww create <filepath>"
                << "\n";
      return 1;
    }

    fs::path filePath = cmdArgs[0];
    std::string templateModifier = "";

    if (cmdArgs.size() == 2)
    {
      templateModifier = cmdArgs[1];
    }

    // special case for pseudo modifiers.
    // Like "date" will create a file with the current date prepending filename
    if (templateModifier == "date")
    {
      std::string date = deps.get_date_yyyymmdd();
      std::string fileName = filePath.filename().string();
      std::string newFileName = date + "-" + fileName;
      filePath.replace_filename(newFileName);

      // processed the pseudo modifier
      templateModifier = "";
    }

    // check if file has extension. If not create a directory instead
    if (filePath.extension().string().empty())
    {
      aww::Result createDirectoryResult = create_new_directory_scenario(filePath, deps);
      if (createDirectoryResult.is_failed())
      {
        std::cout << createDirectoryResult.error();
        return 1;
      }
      return 0;
    }

    aww::Result createResult = try_create_file_by_path(filePath, deps);
    if (createResult.is_failed())
    {
      std::cout << "Failed to create file: " << createResult.error() << "; tag=44f628f70vo\n";
      return 1;
    }

    fs::path awwExecutablePath = deps.fs_get_current_executable_path();
    fs::path awwExecutableDir = std::filesystem::absolute(awwExecutablePath.parent_path());
    fs::path awwCreateTemplatesDir = awwExecutableDir / "aww-create-aww" / "templates";

    fs::path templatePath;
    aww::Result findExistingTemplateResult = assume_template_path(awwCreateTemplatesDir, filePath, templateModifier, templatePath, deps);

    if (findExistingTemplateResult.is_failed())
    {
      std::cout << findExistingTemplateResult.error() << "; tag=de702q38ud3\n";
      return 1;
    }


    std::cout << "assumed templatePath: " << templatePath << "\n";

    bool outIsTemplateFileExists = deps.fs_exists(templatePath);

    // At this point, an empty file has been created.
    // here we check if a template file exists for the file extension
    // if it does, we copy the template file to the new file
    // otherwise, just keep the empty file
    if (outIsTemplateFileExists)
    {
      aww::Result createFileFromTemplateResult = append_template_content_to_new_file_scenario(templatePath, filePath, deps);
      if (createFileFromTemplateResult.is_failed())
      {
        std::cout << createFileFromTemplateResult.error();
        return 1;
      }
    }
    else
    {
      std::cout << "No template found for file : " << filePath.string() << "\n";
    }

    return 0;
  }


  aww::Result assume_template_path(const std::filesystem::path& awwCreateTemplatesDir, const std::filesystem::path& targetFilePath, const std::string& templateModifier, std::filesystem::path& outTemplatePath, aww_create_io_dependencies_interface &deps) {
    std::filesystem::path assumedTemplatePath;

    // get file extension from filePath
    std::string fileExtensionWithDot = targetFilePath.extension().string();

    if (!templateModifier.empty())
    {
      assumedTemplatePath = awwCreateTemplatesDir / ("template-" + templateModifier + fileExtensionWithDot);

      bool outIsTemplateFileExists = deps.fs_exists(assumedTemplatePath);
      if (!outIsTemplateFileExists)
      {
        assumedTemplatePath = awwCreateTemplatesDir / ("template" + fileExtensionWithDot);
      }
    }
    else
    {
      assumedTemplatePath = awwCreateTemplatesDir / ("template" + fileExtensionWithDot);
    }

    outTemplatePath = assumedTemplatePath;
    return aww::Result::ok();
  }

  aww::Result create_new_directory_scenario(const std::filesystem::path& filePath, aww_create_io_dependencies_interface &deps) {
    std::cout << "Creating directory: " << filePath << "\n";
    bool outIsDirectoryExists = deps.fs_exists(filePath);

    if (outIsDirectoryExists)
    {
      std::cout << "Directory already exists: " << filePath << "\n";
      std::string errorMessage = "Directory already exists: " + filePath.string() + "; tag=y5k2qwmd7kv\n";
      return aww::Result::ok();
    }

    if (!deps.fs_create_directories(filePath))
    {
      std::string errorMessage =
        "Failed to create directory: '"+
        filePath.string() +
        "'; tag=evmmi0npk45\n";

      return aww::Result::fail(errorMessage);
    }

    std::cout << "Created directory: " << filePath << "\n";
    return aww::Result::ok();
  }


  aww::Result append_template_content_to_new_file_scenario(const std::filesystem::path& templatePath, const std::filesystem::path& filePath, aww_create_io_dependencies_interface &deps)
  {
      std::cout << "Creating file from template: " << filePath << "\n";

      std::vector<std::string> templateLines;
      std::vector<std::string> newFileLines;

      if (aww::Result res = deps.fs_read_lines(templatePath, templateLines); res.is_failed())
      {
        std::string readFailedMessage = "Failed to read template file: " +
          res.error() +
          "File path: '" + templatePath.string() + "'" +
          "; tag=0bo9nvppdbh\n";

        return aww::Result::fail(readFailedMessage);
      }

      // Here is a heuristic to determine if a line has a template variable
      // an example is "___FILE_NAME___"
      // has 3 underscores, at least 4 characters for reasonable name, 3 underscores
      constexpr size_t MinLineLenWithVariableHeuristic = 10;
      constexpr char VariableStartOrEndToken[] = "___";
      constexpr size_t StartStopTokenLen = 3;

      constexpr char TOKEN_FILE_NAME[] = "FILE_NAME";
      constexpr char CURRENT_DATE[] = "CURRENT_DATE";
      constexpr char RANDOM_INSPIRATION[] = "RANDOM_INSPIRATION";
      constexpr char CAPITALIZED_FILE_NAME[] = "CAPITALIZED_FILE_NAME";

      const std::string TargetFileName = filePath.stem().string();

      // Capitalized TargetFileName
      const std::string CapitalizedTargetFileName = aww::string::to_upper(
        aww::string::to_valid_identifier(TargetFileName)
      );

      for (std::string& line : templateLines)
      {
        if (line.length() >= MinLineLenWithVariableHeuristic)
        {
          // search VariableStartOrEndToken in line

          size_t offsetPos = 0;
          size_t variablePos = 0;
          size_t nextTokenPos = 0;

          size_t tokenPos = line.find(VariableStartOrEndToken, offsetPos);

          while (tokenPos != std::string::npos)
          {
            variablePos = tokenPos + StartStopTokenLen;
            nextTokenPos = line.find(VariableStartOrEndToken, variablePos);

            // if there is no end token, set the offset to where the start token
            // ends and continue
            if (nextTokenPos == std::string::npos)
            {
              offsetPos = tokenPos + StartStopTokenLen;
              tokenPos = line.find(VariableStartOrEndToken, offsetPos);
              continue;
            }
            // here we maybe have a variable name between the start and end tokens
            // get the variable name as a substring
            std::string variableName = line.substr(variablePos, nextTokenPos - variablePos);

            if (variableName == TOKEN_FILE_NAME)
            {
              // replace ___FILE_NAME___ with the file name
              const std::string replacement = TargetFileName;
              line.replace(
                  tokenPos,
                  nextTokenPos + StartStopTokenLen - tokenPos,
                  replacement);
            }
            else if (variableName == CAPITALIZED_FILE_NAME)
            {
              // replace ___CAPITALIZED_FILE_NAME___ with the file name
              const std::string replacement = CapitalizedTargetFileName;
              line.replace(
                  tokenPos,
                  nextTokenPos + StartStopTokenLen - tokenPos,
                  replacement);
            }
            else if (variableName == CURRENT_DATE)
            {
              // replace ___CURRENT_DATE___ with the current date in YYYYMMDD format
              const std::string replacement = aww::date::get_date_YYYYMMDD();
              line.replace(
                  tokenPos,
                  nextTokenPos + StartStopTokenLen - tokenPos,
                  replacement);
            }
            else if (variableName == RANDOM_INSPIRATION)
            {
              // replace ___RANDOM_INSPIRATION___ with a random inspiration
              const std::string replacement = aww::inspiration::getInspiration();
              line.replace(
                  tokenPos,
                  nextTokenPos + StartStopTokenLen - tokenPos,
                  replacement);
            }
            tokenPos = line.find(VariableStartOrEndToken, offsetPos);
          }
        }
        newFileLines.push_back(line);
      }

      if (aww::Result res = deps.fs_write_lines(filePath, newFileLines); res.is_failed())
      {
        std::string writeErrorMessage =
          "Failed to write file: " +
          res.error() +
          "; tag=wzogmbwb8w0\n";

        return aww::Result::fail(writeErrorMessage);
      }

      return aww::Result::ok();
  }

  aww::Result try_create_file_by_path(const fs::path &filePath, aww_create_io_dependencies_interface &deps)
  {
    // if file exists, return error

    bool outFileExists = deps.fs_exists(filePath);
    if (outFileExists)
    {
      return aww::Result::fail("File already exists: '" + filePath.string() + "'");
    }

    std::vector<std::string> filePathParts;
    // traverse path and create directories
    fs::path parentPath;

    for (auto &p : filePath)
    {
      filePathParts.push_back(p.string());
    }

    if (filePathParts.size() == 0)
    {
      return aww::Result::fail(
          "Invalid path: '" + filePath.string() + "'");
    }
    else if (filePathParts.size() == 1)
    {
      // create an empty file
      return deps.fs_create_empty_file(filePath);
    }
    else
    {
      // create directories
      for (std::size_t i = 0; i < filePathParts.size() - 1; i++)
      {
        parentPath /= filePathParts[i];

        bool isDirectoryExists = deps.fs_exists(parentPath);
        if (!isDirectoryExists)
        {
          if (!deps.fs_create_directories(parentPath))
          {
            return aww::Result::fail(
              "Failed to create directory: '" + parentPath.string() + "' tag=07zimuhjpgg");
          }
        }
      }
      // create file
      std::cout << "Creating file: " << filePath << "\n";
      return deps.fs_create_empty_file(filePath);
    }
  }
}

```

## File content `tests\test-aww-create.cpp`:

```cpp
#include "doctest/doctest.h"
#include <string>
#include <vector>
#include <filesystem>

#include "aww-common.hpp"
#include "internal/aww-create.hpp"

class aww_create_io_dependencies_stub : public aww::internal::aww_create::aww_create_io_dependencies_interface
{
public:
  std::function<std::string(int)>
      get_date_yyyymmdd_stub = [this]([[maybe_unused]] int count)
  {
    return "2023-06-02";
  };
  int get_date_yyyymmdd_called = 0;

  std::function<std::filesystem::path()>
      fs_get_current_executable_path_stub = [this]()
  {
    return std::filesystem::path();
  };
  int fs_get_current_executable_path_called = 0;

  std::function<bool(int, const std::filesystem::path&)>
      fs_exists_stub = [this]([[maybe_unused]] int callCount, [[maybe_unused]] const std::filesystem::path &target) -> bool
  {
    return true;
  };
  int fs_exists_called = 0;

  std::function<bool(int, const std::filesystem::path &)>
      fs_create_directories_stub = [this]([[maybe_unused]] int callCount, [[maybe_unused]] const std::filesystem::path &path) -> bool
  {
    return true;
  };
  int fs_create_directories_called = 0;

  std::function<aww::Result(int, const std::filesystem::path &)>
      fs_create_empty_file_stub = [this]([[maybe_unused]] int callCount, [[maybe_unused]] const std::filesystem::path &path) -> aww::Result
  {
    return aww::Result::ok();
  };
  int fs_create_empty_file_called = 0;

  std::function<aww::Result(int, const std::filesystem::path &, std::vector<std::string> &)>
      fs_read_lines_stub = [this]([[maybe_unused]] int callCount, [[maybe_unused]] const std::filesystem::path &filePath, [[maybe_unused]] std::vector<std::string> &outFileLines) -> aww::Result
  {
    return aww::Result::ok();
  };
  int fs_read_lines_called = 0;

  std::function<aww::Result(int, const std::filesystem::path &, const std::vector<std::string> &)>
      fs_write_lines_stub = [this]([[maybe_unused]] int callCount, [[maybe_unused]] const std::filesystem::path &filePath, [[maybe_unused]] const std::vector<std::string> &lines) -> aww::Result
  {
    return aww::Result::ok();
  };
  int fs_write_lines_called = 0;

  std::function<void(int, const std::string &, const std::string &)>
      show_notification_stub = [this]([[maybe_unused]] int callCount, [[maybe_unused]] const std::string &title, [[maybe_unused]] const std::string &message) {

      };
  int show_notification_called = 0;

  std::string get_date_yyyymmdd() override
  {
    get_date_yyyymmdd_called += 1;
    return get_date_yyyymmdd_stub(get_date_yyyymmdd_called);
  }

  std::filesystem::path fs_get_current_executable_path() override
  {
    fs_get_current_executable_path_called += 1;
    return fs_get_current_executable_path_stub();
  }

  inline bool fs_exists(const std::filesystem::path &target) override
  {
    fs_exists_called += 1;
    return fs_exists_stub(fs_exists_called, target);
  }

  bool fs_create_directories(const std::filesystem::path &path) override
  {
    fs_create_directories_called += 1;
    return fs_create_directories_stub(fs_create_directories_called, path);
  }

  aww::Result fs_create_empty_file(const std::filesystem::path &path) override
  {
    fs_create_empty_file_called += 1;
    return fs_create_empty_file_stub(fs_create_empty_file_called, path);
  }

  aww::Result fs_read_lines(const std::filesystem::path &filePath, std::vector<std::string> &outFileLines) override
  {
    fs_read_lines_called += 1;
    return fs_read_lines_stub(fs_read_lines_called, filePath, outFileLines);
  }

  aww::Result fs_write_lines(const std::filesystem::path &filePath, const std::vector<std::string> &lines) override
  {
    fs_write_lines_called += 1;
    return fs_write_lines_stub(fs_write_lines_called, filePath, lines);
  }

  void show_notification(const std::string &title, const std::string &message) override
  {
    show_notification_called += 1;
    show_notification_stub(show_notification_called, title, message);
  }
};

TEST_CASE("aww::internal::aww_create::assume_template_path")
{

  aww_create_io_dependencies_stub ioDependencies;
  const std::filesystem::path awwCreateTemplatesDir = "template-dir";
  const std::filesystem::path targetFilePath = "/tmp/target-file.md";
  std::string templateModifier = "hello";

  std::filesystem::path outTemplatePath;

  SUBCASE("Valid template path with modifier exists")
  {
    // ARRANGE
    ioDependencies.fs_exists_stub = [&](
                                        [[maybe_unused]] int callCount,
                                        [[maybe_unused]] const std::filesystem::path &target) -> bool
    {
      // check if template file exists
      if (target.filename() == "template-hello.md")
      {
        return true;
      }
      return false;
    };

    // ACT
    aww::Result result = assume_template_path(
        awwCreateTemplatesDir,
        targetFilePath,
        templateModifier,
        outTemplatePath,
        ioDependencies);

    // ASSERT
    std::filesystem::path outTemplatePathExpected = awwCreateTemplatesDir / "template-hello.md";
    CHECK_MESSAGE(
        outTemplatePath == outTemplatePathExpected,
        "outTemplatePath should be '" << outTemplatePathExpected << "' but was '" << outTemplatePath << "'");
  }

  SUBCASE("template path with modifier does not exist")
  {
    // ARRANGE
    ioDependencies.fs_exists_stub = [&](
                                        [[maybe_unused]] int callCount,
                                        [[maybe_unused]] const std::filesystem::path &target) -> bool
    {
      return false;
    };

    // ACT
    aww::Result result = assume_template_path(
        awwCreateTemplatesDir,
        targetFilePath,
        templateModifier,
        outTemplatePath,
        ioDependencies);

    // ASSERT
    std::filesystem::path outTemplatePathExpected = awwCreateTemplatesDir / "template.md";
    CHECK_MESSAGE(
        outTemplatePath == outTemplatePathExpected,
        "outTemplatePath should be '" << outTemplatePathExpected << "' but was '" << outTemplatePath << "'");
  }
  SUBCASE("Valid template path without modifier")
  {
    templateModifier = "";
    // ARRANGE
    ioDependencies.fs_exists_stub = [&](
                                        [[maybe_unused]] int callCount,
                                        [[maybe_unused]] const std::filesystem::path &target) -> bool
    {
      return false;
    };

    // ACT
    aww::Result result = assume_template_path(
        awwCreateTemplatesDir,
        targetFilePath,
        templateModifier,
        outTemplatePath,
        ioDependencies);

    // ASSERT
    std::filesystem::path outTemplatePathExpected = awwCreateTemplatesDir / "template.md";
    CHECK_MESSAGE(
        outTemplatePath == outTemplatePathExpected,
        "outTemplatePath should be '" << outTemplatePathExpected << "' but was '" << outTemplatePath << "'");
  }
}

TEST_CASE("aww::internal::aww_create::try_create_file_by_path")
{
  aww_create_io_dependencies_stub ioDependencies;
  std::filesystem::path filePath = "/tmp/newfile.txt";

  SUBCASE("File already exists")
  {
    // ARRANGE
    ioDependencies.fs_exists_stub = [&](
                                        [[maybe_unused]] int callCount,
                                        [[maybe_unused]] const std::filesystem::path &target) -> bool
    {
      return true;
    };

    // ACT
    aww::Result result = try_create_file_by_path(filePath, ioDependencies);

    // ASSERT
    CHECK_MESSAGE(
        result.is_failed(),
        "Result should be failed because file already exists"
    );
  }

  SUBCASE("Invalid path")
  {
    // ARRANGE
    ioDependencies.fs_exists_stub = [&](
                                        [[maybe_unused]] int callCount,
                                        [[maybe_unused]] const std::filesystem::path &target) -> bool
    {
      return false;
    };

    // ACT
    aww::Result result = try_create_file_by_path("", ioDependencies);

    // ASSERT
    CHECK_MESSAGE(
        result.is_failed(),
        "Result should be failed because path is invalid"
    );
  }

  SUBCASE("Create file in existing directory")
  {
    // ARRANGE
    ioDependencies.fs_exists_stub = [&](
                                        [[maybe_unused]] int callCount,
                                        [[maybe_unused]] const std::filesystem::path &target) -> bool
    {
      return false;
    };

    ioDependencies.fs_create_empty_file_stub = [&](
                                                [[maybe_unused]] int callCount,
                                                [[maybe_unused]] const std::filesystem::path& path) -> aww::Result
    {
      return aww::Result::ok();
    };

    // ACT
    aww::Result result = try_create_file_by_path(filePath, ioDependencies);

    // ASSERT
    CHECK_MESSAGE(
        result.is_ok(),
        "Result should be successful"
    );
  }

  SUBCASE("Create file with nested directories")
  {
    // ARRANGE
    ioDependencies.fs_exists_stub = [&](
                                        [[maybe_unused]] int callCount,
                                        [[maybe_unused]] const std::filesystem::path &target) -> bool
    {
      return false;
    };

    ioDependencies.fs_create_directories_stub = [&](
                                                  [[maybe_unused]] int callCount,
                                                  [[maybe_unused]] const std::filesystem::path& path) -> bool
    {
      return true;
    };

    ioDependencies.fs_create_empty_file_stub = [&](
                                                [[maybe_unused]] int callCount,
                                                [[maybe_unused]] const std::filesystem::path& path) -> aww::Result
    {
      return aww::Result::ok();
    };

    // ACT
    aww::Result result = try_create_file_by_path("/tmp/nested/directory/newfile.txt", ioDependencies);

    // ASSERT
    CHECK_MESSAGE(
        result.is_ok(),
        "Result should be successful"
    );
  }
}

TEST_CASE("aww::internal::aww_create::create_new_directory_scenario")
{
  aww_create_io_dependencies_stub ioDependencies;
  std::filesystem::path filePath = "/tmp/new_directory";

  SUBCASE("Directory already exists")
  {
    // ARRANGE
    ioDependencies.fs_exists_stub = [&](
                                        [[maybe_unused]] int callCount,
                                        [[maybe_unused]] const std::filesystem::path &target) -> bool
    {
      return false;
    };

    // ACT
    aww::Result result = create_new_directory_scenario(filePath, ioDependencies);

    // ASSERT
    CHECK_MESSAGE(
        result.is_ok(),
        "Result should be successful even if directory already exists"
    );
  }

  SUBCASE("Create directory")
  {
    // ARRANGE
    ioDependencies.fs_exists_stub = [&](
                                        [[maybe_unused]] int callCount,
                                        [[maybe_unused]] const std::filesystem::path &target) -> bool
    {
      return false;
    };

    ioDependencies.fs_create_directories_stub = [&](
                                                  [[maybe_unused]] int callCount,
                                                  [[maybe_unused]] const std::filesystem::path& path) -> bool
    {
      return true;
    };

    // ACT
    aww::Result result = create_new_directory_scenario(filePath, ioDependencies);

    // ASSERT
    CHECK_MESSAGE(
        result.is_ok(),
        "Result should be successful"
    );
  }

  SUBCASE("Failed to create directory")
  {
    // ARRANGE
    ioDependencies.fs_exists_stub = [&](
                                        [[maybe_unused]] int callCount,
                                        [[maybe_unused]] const std::filesystem::path &target) -> bool
    {
      return false;
    };

    ioDependencies.fs_create_directories_stub = [&](
                                                  [[maybe_unused]] int callCount,
                                                  [[maybe_unused]] const std::filesystem::path& path) -> bool
    {
      return false;
    };

    // ACT
    aww::Result result = create_new_directory_scenario(filePath, ioDependencies);

    // ASSERT
    CHECK_MESSAGE(
        result.is_failed(),
        "Result should be failed if failed to create directory"
    );
  }
}

TEST_CASE("aww::internal::aww_create::append_template_content_to_new_file_scenario")
{
  aww_create_io_dependencies_stub ioDependencies;
  std::filesystem::path templatePath = "/path/to/template.txt";
  std::filesystem::path filePath = "/path/to/new_file.txt";

  SUBCASE("Failed to read template file")
  {
    // ARRANGE
    ioDependencies.fs_read_lines_stub = [&](
        [[maybe_unused]] int callCount,
        [[maybe_unused]] const std::filesystem::path& filePath,
        [[maybe_unused]] std::vector<std::string>& outFileLines) -> aww::Result
    {
      return aww::Result::fail("Failed to read template file");
    };

    // ACT
    aww::Result result = append_template_content_to_new_file_scenario(templatePath, filePath, ioDependencies);

    // ASSERT
    CHECK_MESSAGE(
        result.is_failed(),
        "Result should be failed if failed to read template file"
    );
  }

  SUBCASE("Successful template content append - processing")
  {
    // ARRANGE
    std::vector<std::string> newFileLines;
    std::vector<std::string> templateLines = {"Existing Line 1", "___FILE_NAME___", "___CAPITALIZED_FILE_NAME___"};
    std::vector<std::string> expectedLines = {"Existing Line 1", "new_file", "NEW_FILE"};

    ioDependencies.fs_read_lines_stub = [&](
        [[maybe_unused]] int callCount,
        [[maybe_unused]] const std::filesystem::path& filePath,
        [[maybe_unused]] std::vector<std::string>& outFileLines) -> aww::Result
    {
      outFileLines = templateLines;
      return aww::Result::ok();
    };

    ioDependencies.fs_write_lines_stub = [&](
        [[maybe_unused]] int callCount,
        [[maybe_unused]] const std::filesystem::path& filePath,
        [[maybe_unused]] const std::vector<std::string>& lines) -> aww::Result
    {
      newFileLines = lines;
      return aww::Result::ok();
    };

    // ACT
    aww::Result result = append_template_content_to_new_file_scenario(templatePath, filePath, ioDependencies);

    // ASSERT
    CHECK_MESSAGE(
        result.is_ok(),
        "Result should be successful"
    );

    CHECK_MESSAGE(
        newFileLines == expectedLines,
        "New file lines should match the expected lines. "
        << "\nExpected:\n" <<  aww::string::join(expectedLines, "\n")
        << "\nActual:\n" << aww::string::join(newFileLines, "\n")
    );
  }

  SUBCASE("Failed to write file")
  {
    // ARRANGE
    std::vector<std::string> templateLines = {"Line 1", "Line 2", "Line 3"};

    ioDependencies.fs_read_lines_stub = [&](
        [[maybe_unused]] int callCount,
        [[maybe_unused]] const std::filesystem::path& filePath,
        [[maybe_unused]] std::vector<std::string>& outFileLines) -> aww::Result
    {
      outFileLines = templateLines;
      return aww::Result::ok();
    };

    ioDependencies.fs_write_lines_stub = [&](
        [[maybe_unused]] int callCount,
        [[maybe_unused]] const std::filesystem::path& filePath,
        [[maybe_unused]] const std::vector<std::string>& lines) -> aww::Result
    {
      return aww::Result::fail("Failed to write file");
    };

    // ACT
    aww::Result result = append_template_content_to_new_file_scenario(templatePath, filePath, ioDependencies);

    // ASSERT
    CHECK_MESSAGE(
        result.is_failed(),
        "Result should be failed if failed to write file"
    );
  }
}
```



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
      if (createDirectoryResult.isFailed())
      {
        std::cout << createDirectoryResult.error();
        return 1;
      }
      return 0;
    }

    aww::Result createResult = try_create_file_by_path(filePath, deps);
    if (createResult.isFailed())
    {
      std::cout << "Failed to create file: " << createResult.error() << "; tag=44f628f70vo\n";
      return 1;
    }

    fs::path awwExecutablePath = deps.fs_get_current_executable_path();
    fs::path awwExecutableDir = std::filesystem::absolute(awwExecutablePath.parent_path());
    fs::path awwCreateTemplatesDir = awwExecutableDir / "aww-create-aww" / "templates";

    fs::path templatePath;
    aww::Result findExistingTemplateResult = assume_template_path(awwCreateTemplatesDir, filePath, templateModifier, templatePath, deps);

    if (findExistingTemplateResult.isFailed())
    {
      std::cout << findExistingTemplateResult.error() << "; tag=de702q38ud3\n";
      return 1;
    }


    std::cout << "assumed templatePath: " << templatePath << "\n";

    bool outIsTemplateFileExists = false;
    aww::Result templateFileExistsResult = deps.fs_exists(templatePath, outIsTemplateFileExists);

    if (templateFileExistsResult.isFailed())
    {
      std::cout << "Failed to check if template file exists: " << templateFileExistsResult.error() << "; tag=htok5anj6gq\n";
      return 1;
    }

    // At this point, an empty file has been created.
    // here we check if a template file exists for the file extension
    // if it does, we copy the template file to the new file
    // otherwise, just keep the empty file
    if (outIsTemplateFileExists)
    {
      aww::Result createFileFromTemplateResult = append_template_content_to_new_file_scenario(templatePath, filePath, deps);
      if (createFileFromTemplateResult.isFailed())
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

      bool outIsTemplateFileExists = false;
      aww::Result templateFileExistsResult = deps.fs_exists(assumedTemplatePath, outIsTemplateFileExists);

      if (templateFileExistsResult.isFailed())
      {
        std::string errorMessage = "Failed to check if template file exists: " + templateFileExistsResult.error();
        return aww::Result::fail(errorMessage);
      }

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
    bool outIsDirectoryExists = false;

    aww::Result directoryAlreadyExistResult = deps.fs_exists(filePath, outIsDirectoryExists);

    if (directoryAlreadyExistResult.isFailed())
    {
      std::string errorMessage =  "Failed to check if directory exists: " +
        directoryAlreadyExistResult.error() + "; tag=qu0rbfob4ey\n";
      return aww::Result::fail(errorMessage);
    }

    if (outIsDirectoryExists)
    {
      std::cout << "Directory already exists: " << filePath << "\n";
      std::string errorMessage = "Directory already exists: " + filePath.string() + "; tag=y5k2qwmd7kv\n";
      return aww::Result::ok();
    }

    aww::Result createDirResult = deps.fs_create_directories(filePath);
    if (createDirResult.isFailed())
    {
      std::string errorMessage = "Failed to create directory: " + createDirResult.error() + "; tag=evmmi0npk45\n";
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

      aww::Result readTemplateResult = deps.fs_read_lines(templatePath, templateLines);

      if (readTemplateResult.isFailed())
      {
        std::string readFailedMessage = "Failed to read template file: " +
          readTemplateResult.error() +
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
      constexpr char CPP_HEADER_FILE_NAME[] = "CPP_HEADER_FILE_NAME";

      const std::string TargetFileName = filePath.stem().string();

      // Capitalized TargetFileName
      const std::string CapitalizedTargetFileName = aww::string::toupper(
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
            else if (variableName == CPP_HEADER_FILE_NAME)
            {
              // replace ___CPP_HEADER_FILE_NAME___ with the file name
              const std::string replacement = CapitalizedTargetFileName;
              line.replace(
                  tokenPos,
                  nextTokenPos + StartStopTokenLen - tokenPos,
                  replacement);
            }
            else if (variableName == CURRENT_DATE)
            {
              // replace ___CURRENT_DATE___ with the current date in YYYYMMDD format
              const std::string replacement = aww::date::getDateYYYYMMDD();
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

      aww::Result writeLinesResult = deps.fs_write_lines(filePath, newFileLines);

      if (writeLinesResult.isFailed())
      {
        std::string writeErrorMessage = "Failed to write file: " +  writeLinesResult.error() +  "; tag=wzogmbwb8w0\n";
        return aww::Result::fail(writeErrorMessage);
      }

      return aww::Result::ok();
  }

  aww::Result try_create_file_by_path(const fs::path &filePath, aww_create_io_dependencies_interface &deps)
  {
    // if file exists, return error

    bool outFileExists = false;
    aww::Result fsExistsResult = deps.fs_exists(filePath, outFileExists);

    if (fsExistsResult.isFailed())
    {
      return fsExistsResult;
    }

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

        bool isDirectoryExists = false;
        aww::Result fsDirExistsResult = deps.fs_exists(parentPath, isDirectoryExists);

        if (fsDirExistsResult.isFailed())
        {
          return fsDirExistsResult;
        }

        if (!isDirectoryExists)
        {
          aww::Result createDirResult = deps.fs_create_directories(parentPath);

          if (createDirResult.isFailed())
          {
            return aww::Result::fail(
              "Failed to create directory: '" + parentPath.string() + "':\n " +
              createDirResult.error());
          }
        }
      }
      // create file
      std::cout << "Creating file: " << filePath << "\n";

      return deps.fs_create_empty_file(filePath);
    }
  }
}

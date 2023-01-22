#include <iostream>
#include <fstream>

#include <string>
#include <vector>
#include <filesystem>


#include "aww-common.hpp"

namespace fs = std::filesystem;

aww::result_t tryCreateFileByPath(const fs::path&);

int main(int argc, char **argv)
{
  std::vector<std::string> cmdArgs(argv, argv + argc);
  cmdArgs.erase(cmdArgs.begin()); // remove first element

  if (cmdArgs.size() == 0) {
    std::cout << "Usage: aww create <filepath>" << "\n";
    return 1;
  }

  const fs::path filePath = cmdArgs[0];
  if (cmdArgs.size() == 1) {
    aww::result_t createResult = tryCreateFileByPath(filePath);
    if (aww::failed(createResult)) {
      std::cout << aww::makeError("Failed to create file", createResult) << "\n";
      return 1;
    }
  }

  fs::path awwExecutablePath = aww::fs::getCurrentExecutablePath();
  fs::path awwExecutableDir = std::filesystem::absolute(awwExecutablePath.parent_path());
  fs::path awwCreateTemplates = awwExecutableDir / "aww-create-aww" / "templates";

  // get file extension from filePath
  std::string fileExtensionWithDot = filePath.extension().string();
  fs::path templatePath = awwCreateTemplates / ("template" + fileExtensionWithDot);

  std::cout << "templatePath: " << templatePath << "\n";

  if (fs::exists(templatePath)) {
    std::cout << "Creating file from template: " << filePath << "\n";
    std::ifstream templateFile(templatePath);
    std::ofstream file(filePath);

    // read file line by line and output to file
    std::string line;

    // Here is a heuristic to determine if a line has a template variable
    // an example is "___FILE_NAME___"
    // has 3 underscores, at least 4 characters for reasonable name, 3 underscores
    constexpr size_t MinLineLenWithVariableHeuristic = 10;
    constexpr char VariableStartOrEndToken[] = "___";
    constexpr size_t StartStopTokenLen = 3;

    constexpr char TOKEN_FILE_NAME[] = "FILE_NAME";
    constexpr char CURRENT_DATE[] = "CURRENT_DATE";

    while (std::getline(templateFile, line)) {

      if (line.length() >= MinLineLenWithVariableHeuristic) {
        // search VariableStartOrEndToken in line

        size_t offsetPos = 0;
        size_t variablePos = 0;
        size_t nextTokenPos = 0;

        size_t tokenPos = line.find(VariableStartOrEndToken, offsetPos);

        while (tokenPos  != std::string::npos) {
          variablePos = tokenPos + StartStopTokenLen;
          nextTokenPos = line.find(VariableStartOrEndToken, variablePos);

          // if there is no end token, set the offset to where the start token
          // ends and continue
          if (nextTokenPos == std::string::npos) {
            offsetPos = tokenPos + StartStopTokenLen;
            tokenPos = line.find(VariableStartOrEndToken, offsetPos);
            continue;
          }
          // here we maybe have a variable name between the start and end tokens
          // get the variable name as a substring
          std::string variableName = line.substr(variablePos, nextTokenPos - variablePos);

          if (variableName == TOKEN_FILE_NAME) {
            // replace ___FILE_NAME___ with the file name
            const std::string replacement = filePath.stem().string();
            line.replace(
              tokenPos,
              nextTokenPos + StartStopTokenLen - tokenPos,
              replacement);
          } else if (variableName == CURRENT_DATE) {
            // replace ___CURRENT_DATE___ with the current date in YYYYMMDD format
            const std::string replacement = aww::date::getDateYYYYMMDD();
            line.replace(
              tokenPos,
              nextTokenPos + StartStopTokenLen - tokenPos,
              replacement);
          }
          tokenPos = line.find(VariableStartOrEndToken, offsetPos);
        }
      }

      file << line << "\n";
    }

    // file << templateFile.rdbuf();
    templateFile.close();
    file.close();
  } else {
    std::cout << "No template found for file extension: " << fileExtensionWithDot << "\n";
  }

  return 0;
}

aww::result_t tryCreateFileByPath(const fs::path &filePath)
{

  // if file exists, return error
  if (fs::exists(filePath)) {
    return std::make_tuple(false, "File already exists");
  }

  std::vector<std::string> filePathParts;
  // traverse path and create directories
  fs::path parentPath;

  for (auto &p : filePath) {
    filePathParts.push_back(p.string());
  }

  if (filePathParts.size() == 0) {
    return std::make_tuple(false, "Invalid path: '" + filePath.string() + "'");
  } else if (filePathParts.size() == 1) {
    // create file
    std::ofstream file(filePath);
    file.close();
    return std::make_tuple(true, "");
  } else {
    // create directories
    for (std::size_t i = 0; i < filePathParts.size() - 1; i++) {
      parentPath /= filePathParts[i];
      if (!fs::exists(parentPath)) {
        if (!fs::create_directory(parentPath)) {
          return std::make_tuple(
            false,
            "Failed to create directory: '" + parentPath.string() + "'");
        }
      }
    }
    // create file
    std::cout << "Creating file: " << filePath << "\n";
    std::ofstream file(filePath);
    file.close();
  }

  // std::cout << "Creating file: " << path << "\n";
  return std::make_tuple(true, "");
}

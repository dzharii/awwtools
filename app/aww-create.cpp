#include <iostream>
#include <fstream>

#include <string>
#include <vector>
#include <filesystem>


#include "aww-common.hpp"

namespace fs = std::filesystem;

aww::result_t tryCreateFileByPath(const std::string&);

int main(int argc, char **argv)
{
  std::vector<std::string> cmdArgs(argv, argv + argc);
  cmdArgs.erase(cmdArgs.begin()); // remove first element

  if (cmdArgs.size() == 0) {
    std::cout << "Usage: aww create <filepath>" << "\n";
    return 1;
  }

  if (cmdArgs.size() == 1) {
    aww::result_t createResult = tryCreateFileByPath(cmdArgs[0]);
    if (aww::failed(createResult)) {
      std::cout << aww::makeError("Failed to create file", createResult) << "\n";
      return 1;
    }
  }

  return 0;
}

aww::result_t tryCreateFileByPath(const std::string &path)
{

  // check if path is valid path
  fs::path filePath(path);

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
    return std::make_tuple(false, "Invalid path: '" + path + "'");
  } else if (filePathParts.size() == 1) {
    // create file
    std::ofstream file(path);
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

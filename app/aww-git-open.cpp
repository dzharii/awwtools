// Executables must have the following defined if the library contains
// doctest definitions. For builds with this disabled, e.g. code shipped to
// users, this can be left out.
#ifdef ENABLE_DOCTEST_IN_LIBRARY
#define DOCTEST_CONFIG_IMPLEMENT
#include "doctest/doctest.h"
#endif

#include <iostream>
#include <string>
#include <filesystem>
#include <fstream>
#include <istream>
#include <regex>

#include "aww-common.hpp"

namespace fs = std::filesystem;

bool findGitRepo(const fs::path &, fs::path &);
bool tryConvertToGitUrl(const std::string &, std::string &);
aww::result_t tryFindRepositoryUrlInGitConfig(std::istream &, std::string &);
aww::result_t getRelativeUrlPath(const fs::path &, const fs::path &, std::string &);

/*
 * Simple main program that demonstrates how access
 * CMake definitions (here the version number) from source code.
 */
int main(int argc, char **argv)
{
  std::cout << "Embrace the Aww!" << std::endl;

  if (argc > 2)
  {
    std::cout << "Too many arguments provided" << std::endl;
    return 1;
  }

  // this will open the current directory if no argument is provided
  std::string optionalFileToOpen = ".";
  if (argc == 2)
  {
    optionalFileToOpen = argv[1];
  }


  fs::path currentDir = fs::absolute(fs::current_path());
  fs::path optionalPathAbsolute;

  if (fs::exists(optionalFileToOpen)) {
    optionalPathAbsolute = fs::absolute(optionalFileToOpen);

    // check if it is file or directory
    if (fs::is_directory(optionalPathAbsolute)) {
      currentDir = optionalPathAbsolute;
    } else {
      currentDir = fs::absolute(optionalPathAbsolute.parent_path());
    }
  }

  std::cout << "Optional file to open: " << optionalPathAbsolute << std::endl;
  std::cout << "Current directory: " << currentDir << std::endl;

  fs::path gitRepo;

  bool gitRepoFound = findGitRepo(currentDir, gitRepo);

  if (!gitRepoFound)
  {
    std::cout << "No git repository found in " << currentDir << std::endl;
    return 1;
  }

  std::cout << "Found git repo: " << gitRepo << std::endl;
  fs::path gitConfigPath = gitRepo / ".git" / "config";
  std::cout << "git config path: " << gitConfigPath << std::endl;

  // read line by line
  std::ifstream file(gitConfigPath);
  std::string repoUrl;
  aww::result_t findUrlResult = tryFindRepositoryUrlInGitConfig(file, repoUrl);
  if (aww::failed(findUrlResult))
  {
    std::cout << aww::makeError("Failed to find repository url in git config [tryFindRepositoryUrlInGitConfig]", findUrlResult)
              << std::endl;
    return 1;
  }

  std::cout << "Found url: " << repoUrl << std::endl;

  std::string webUrl;
  bool convertedToWebUrl = tryConvertToGitUrl(repoUrl, webUrl);
  if (!convertedToWebUrl)
  {
    std::cout << "Failed to convert git repo url to web url" << std::endl;
    return 1;
  }

  std::cout << "Converted to web url: " << webUrl << std::endl;

  // extract gitRepo from optionalFileToOpen
  fs::path gitRepoAbsolute = fs::absolute(gitRepo);


  // TODO Error handling and logging

  if (!optionalPathAbsolute.empty())
  {
    std::string webPath;
    aww::result_t webPathConverted = getRelativeUrlPath(gitRepoAbsolute, optionalPathAbsolute, webPath);

    if (aww::failed(webPathConverted))
    {
      std::cout << aww::makeError("Failed to convert path to web url", webPathConverted) << std::endl;
      return 1;
    }

    webUrl = webUrl + "?path=" + webPath;
    std::cout << "Opening file: " << webUrl << std::endl;
  }

  aww::result_t launchFileRes = aww::os::actions::launchFile(webUrl);
  if (aww::failed(launchFileRes))
  {
    std::cout << aww::makeError("Failed to launch file", launchFileRes) << std::endl;
    aww::os::actions::showNotification("aww git open", "Failed to open file in browser");
    return 1;
  }
  std::cout << "Launched file" << std::endl;
  aww::os::actions::showNotification("aww git open", "The file was opened in browser");
  return 0;
}

aww::result_t getRelativeUrlPath(const fs::path &parentAbsPath, const fs::path &childAbsPath, std::string &result)
{
  std::string parentPathStr = parentAbsPath.string();
  std::string childPathStr = childAbsPath.string();
  if (childPathStr.find(parentPathStr) != 0)
  {

    std::string message;
    message += "Child path is not a child of parent path.\n";
    message += "Parent path: '";
    message += parentPathStr + "'\n";
    message += "Child path: '" + childPathStr + "'\n";
    return std::make_tuple(false, message);
  }

  if (childPathStr == parentPathStr) {
    result = "";
    return std::make_tuple(true, "");
  }
      // subtract gitrepo from optionalPathAbsolute
  std::string relativeRepoUrlPath = childPathStr.substr(parentPathStr.length() + 1);

  // replace backslashes with forward slashes
  std::replace(relativeRepoUrlPath.begin(), relativeRepoUrlPath.end(), '\\', '/');
  result = relativeRepoUrlPath;
  return std::make_tuple(true, "");
}

aww::result_t tryFindRepositoryUrlInGitConfig(std::istream &gitConfigStream, std::string &gitSshOrHttpUri)
{
  std::string result = "";
  std::string str;
  bool foundOrigin = false;
  while (std::getline(gitConfigStream, str))
  {
    if (str == "[remote \"origin\"]")
    {
      foundOrigin = true;
      break;
    }
  }

  if (!foundOrigin)
  {
    return std::make_tuple(
        false,
        "Configuration entry for remote origin was not found");
  }

  std::string urlConfigLine;
  bool foundUrl = false;

  while (std::getline(gitConfigStream, str))
  {
    if (str.find("url = ") != std::string::npos)
    {
      // split str on =
      std::string delimiter = "=";
      urlConfigLine = str.substr(str.find(delimiter) + 1);

      // trim whitespace before and after
      urlConfigLine = urlConfigLine.erase(0, urlConfigLine.find_first_not_of(' '));
      urlConfigLine = urlConfigLine.erase(urlConfigLine.find_last_not_of(' ') + 1);

      foundUrl = true;
      std::cout << "urlConfigLine: " << urlConfigLine << std::endl;
      break;
    }
  }
  if (!foundUrl)
  {
    return std::make_tuple(
        false,
        "Configuration entry for remote origin url was not found");
  }
  gitSshOrHttpUri = urlConfigLine;
  return std::make_tuple(true, "");
}

//  Attempts to convert git origin url to a web url
//  git@github.com:dzharii/awwtools.git     => https://github.com/dzharii/awwtools.git
//  https://github.com/dzharii/awwtools.git => https://github.com/dzharii/awwtools.git
bool tryConvertToGitUrl(const std::string &inputUrl, std::string &httpUrl)
{
  if (inputUrl.find("https://") == 0 || inputUrl.find("http://") == 0)
  {
    httpUrl = inputUrl;
    return true;
  }
  else if (inputUrl.find("git@github") == 0)
  {
    std::regex reGithubSsh("^git@github.com:([^/]+)/(\\S+)$");
    std::smatch match;

    if (!std::regex_search(inputUrl, match, reGithubSsh)) {
      return false;
    }

    std::string user = match[1];
    std::string repo = match[2];

    httpUrl = "https://github.com/" + user + "/" + repo;
    return true;
  }
  else if (inputUrl.find("@vs-ssh.visualstudio.com") > 0)
  {
    std::cout << "aaaa";

    std::regex reGithubSsh("^.+?@vs-ssh.visualstudio.com:[^/]+/([^/]+)/([^/]+)/(.+)$");
    std::smatch match;

    if (!std::regex_search(inputUrl, match, reGithubSsh)) {
      return false;
    }

    const std::string user = match[1];
    const std::string project = match[2];
    const std::string repo = match[3];

    httpUrl =
      "https://" + user + ".visualstudio.com/DefaultCollection/" + project + "/_git/" + repo;

    return true;
  }
  return false;
}

bool findGitRepo(const fs::path& dirPath, fs::path &gitRepoPath)
{
  std::cout << "Searching for git repo in: " << dirPath << std::endl;
  fs::path currentDir(dirPath);
  fs::path gitPath = currentDir / ".git";
  std::cout << "gitPath: " << gitPath << std::endl;
  if (fs::exists(gitPath))
  {
    std::cout << "Found git repo in: " << currentDir << std::endl;
    // The parent of the .git directory is the git repo
    gitRepoPath = gitPath.parent_path();
    return true;
  }

  std::cout << "No git repo found in: " << currentDir << std::endl;
  const bool isRoot = currentDir == currentDir.root_path();
  if (!isRoot)
  {
    currentDir = currentDir.parent_path();
    return findGitRepo(currentDir, gitRepoPath);
  }
  std::cout << "Reached root directory, no git repo found." << std::endl;
  return false;
}

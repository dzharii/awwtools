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

bool findGitRepo(std::filesystem::path, std::filesystem::path &);
bool tryConvertToGitUrl(std::string, std::string &);
aww::result_t tryFindRepositoryUrlInGitConfig(std::istream &, std::string &);


// 2022-10-29 BUG: D:\my-github\awwtools\cmake-build\bin\aww-git-open.exe README.md



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

  std::string optionalFileToOpen = "";
  if (argc == 2)
  {
    optionalFileToOpen = argv[1];
  }


  std::filesystem::path currentDir = std::filesystem::current_path();
  std::string optionalPathAbsolute = "";

  if (std::filesystem::exists(optionalFileToOpen)) {
    std::filesystem::path optionalFileToOpenPathAsPath = optionalFileToOpen;
    optionalPathAbsolute = std::filesystem::absolute(optionalFileToOpenPathAsPath).string();

    // check if it is file or directory
    if (std::filesystem::is_directory(optionalFileToOpenPathAsPath)) {
      currentDir = optionalFileToOpenPathAsPath;
    } else {
      currentDir = optionalFileToOpenPathAsPath.parent_path();
    }
  }

  std::cout << "Optional file to open: " << optionalPathAbsolute << std::endl;


  std::filesystem::path gitRepo;

  bool gitRepoFound = findGitRepo(currentDir, gitRepo);

  if (!gitRepoFound)
  {
    std::cout << "No git repository found in " << currentDir << std::endl;
    return 1;
  }

  std::cout << "Found git repo: " << gitRepo << std::endl;
  std::filesystem::path gitConfigPath = gitRepo / ".git" / "config";
  std::cout << "git config path: " << gitConfigPath << std::endl;

  // read line by line
  std::ifstream file(gitConfigPath);
  std::string repoUrl;
  aww::result_t findUrlResult = tryFindRepositoryUrlInGitConfig(file, repoUrl);
  if (aww::failed(findUrlResult))
  {
    std::cout << aww::make_error("Failed to find repository url in git config [tryFindRepositoryUrlInGitConfig]", findUrlResult)
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
  std::string gitRepoAbsolute = std::filesystem::absolute(gitRepo).string();


  // TODO Error handling and logging

  if (!optionalPathAbsolute.empty())
  {

    // cout optionalPathAbsolute and gitRepoAbsolute
    std::cout << "Optional path absolute: " << optionalPathAbsolute << std::endl;
    std::cout << "Git repo absolute: " << gitRepoAbsolute << std::endl;

    // subtract gitrepo from optionalPathAbsolute
    std::string relativeRepoUrlPath = optionalPathAbsolute.substr(gitRepoAbsolute.length() + 1);
    std::cout << "Relative repo url path: " << relativeRepoUrlPath << std::endl;

    // replace backslashes with forward slashes
    std::replace(relativeRepoUrlPath.begin(), relativeRepoUrlPath.end(), '\\', '/');
    std::cout << "Relative repo url path: " << relativeRepoUrlPath << std::endl;

    webUrl = webUrl + "?path=" + relativeRepoUrlPath;
    std::cout << "Opening file: " << webUrl << std::endl;
  }
  //optionalPathAbsolute



  aww::result_t launchFileRes = aww::os::actions::launchFile(webUrl);
  if (aww::failed(launchFileRes))
  {
    std::cout << aww::make_error("Failed to launch file", launchFileRes) << std::endl;
  }
  std::cout << "Launched file" << std::endl;
  return 0;
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
bool tryConvertToGitUrl(std::string inputUrl, std::string &httpUrl)
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
    if (std::regex_search(inputUrl, match, reGithubSsh))
    {
      std::string user = match[1];
      std::string repo = match[2];
      std::string githubUrl = "https://github.com/" + user + "/" + repo;
      httpUrl = githubUrl;
      return true;
    }
    return false;
  }
  return false;
}

bool findGitRepo(std::filesystem::path dirPath, std::filesystem::path &gitRepoPath)
{
  std::cout << "Searching for git repo in: " << dirPath << std::endl;
  std::filesystem::path currentDir(dirPath);
  std::filesystem::path gitPath = currentDir / ".git";
  std::cout << "gitPath: " << gitPath << std::endl;
  if (std::filesystem::exists(gitPath))
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

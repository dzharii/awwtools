
#include <iostream>
#include <string>
#include <sstream>
#include <filesystem>
#include <fstream>
#include <istream>
#include <regex>

#include "aww-common.hpp"
#include "internal/aww-git-open.hpp"

namespace aww::internal::aww_git_open
{
  namespace fs = std::filesystem;

  int aww_git_open_main(const std::vector<std::string> &cmdArgs, aww_git_open_io_dependencies_interface &deps)
  {
    if (cmdArgs.size() > 1)
    {
      std::cout << "Too many arguments provided" << "\n";
      return 1;
    }

    // this will open the current directory if no argument is provided
    std::string optionalFileToOpen = ".";
    if (cmdArgs.size() == 1)
    {
      optionalFileToOpen = cmdArgs[0];
    }

    fs::path currentDir;
    if (aww::Result res = deps.fs_get_current_directory_absolute_path(currentDir); res.is_failed())
    {
      std::cout << "Failed to get current directory"
                << res.error()
                << "\n";
      return 1;
    }

    std::cout << "Current directory tag=b5w2oy8p662: " << currentDir << "\n";

    fs::path optionalPathAbsolute;

    bool optionalFileToOpenExists = false;
    if (aww::Result res = deps.fs_exists(optionalFileToOpen, optionalFileToOpenExists); res.is_failed())
    {
      std::cout << "Failed to check if file exists"
                << res.error()
                << "\n";
      return 1;
    }

    if (optionalFileToOpenExists)
    {

      if (aww::Result res = deps.fs_get_absolute_path(optionalPathAbsolute); res.is_failed())
      {
        std::cout << "Failed to get absolute path"
                  << res.error()
                  << "\n";
        return 1;
      }

      // check if it is file or directory
      bool optionalPathIsDirectory = false;
      if (aww::Result res = deps.fs_is_directory(optionalPathAbsolute, optionalPathIsDirectory); res.is_failed())
      {
        std::cout << "Failed to check if path is directory"
                  << res.error()
                  << "\n";
        return 1;
      }

      if (optionalPathIsDirectory)
      {
        currentDir = optionalPathAbsolute;
      }
      else
      {
        currentDir = optionalPathAbsolute.parent_path();
      }
    }

    std::cout << "Optional file to open: " << optionalPathAbsolute << "\n";
    std::cout << "Current directory: " << currentDir << "\n";

    fs::path gitRepo;

    bool gitRepoFound = find_git_repo(currentDir, gitRepo);

    if (!gitRepoFound)
    {
      std::cout << "No git repository found in " << currentDir << "\n";
      return 1;
    }

    std::cout << "Found git repo: " << gitRepo << "\n";
    fs::path gitConfigPath = gitRepo / ".git" / "config";
    std::cout << "git config path: " << gitConfigPath << "\n";

    // read line by line
    std::ifstream file(gitConfigPath);
    std::string repoUrl;
    aww::Result findUrlResult = try_find_repository_url_in_git_config(file, repoUrl);
    if (findUrlResult.is_failed())
    {
      std::cout << "Failed to find repository url in git config "
                << "[try_find_repository_url_in_git_config]"
                << findUrlResult.error()
                << "\n";
      return 1;
    }

    std::cout << "Found url: " << repoUrl << "\n";

    std::string webUrl;
    bool convertedToWebUrl = try_convert_to_git_url(repoUrl, webUrl);
    if (!convertedToWebUrl)
    {
      std::cout << "Failed to convert git repo url to web url"
                << "\n";
      return 1;
    }

    std::cout << "Converted to web url: " << webUrl << "\n";

    // extract gitRepo from optionalFileToOpen
    fs::path gitRepoAbsolute = fs::absolute(gitRepo);

    // TODO Error handling and logging

    if (!optionalPathAbsolute.empty())
    {
      std::string webPath;
      aww::Result webPathConverted = get_relative_url_path(
          gitRepoAbsolute,
          optionalPathAbsolute,
          webPath);

      if (webPathConverted.is_failed())
      {
        std::cout << "Failed to convert path to web url"
                  << webPathConverted.error()
                  << "\n";
        return 1;
      }

      webUrl = webUrl + "?path=" + webPath;
      std::cout << "Opening file: " << webUrl << "\n";
    }

    aww::Result launchFileRes = aww::os::actions::launch_file(webUrl);
    if (launchFileRes.is_failed())
    {
      std::cout << "Failed to launch file" << launchFileRes.error() << "\n";
      aww::os::actions::show_notification("aww git open", "Failed to open file in browser");
      return 1;
    }
    std::cout << "Launched file"
              << "\n";
    aww::os::actions::show_notification("aww git open", "The file was opened in browser");
    return 0;
  }

  aww::Result get_relative_url_path(const fs::path &parentAbsPath, const fs::path &childAbsPath, std::string &result)
  {
    std::string parentPathStr = parentAbsPath.string();
    std::string childPathStr = childAbsPath.string();
    if (childPathStr.find(parentPathStr) != 0)
    {

      std::stringstream message;
      message << "Child path is not a child of parent path.\n"
              << "Parent path: '" << parentPathStr << "'\n"
              << "Child path: '" << childPathStr << "'\n";
      return aww::Result::fail(message.str());
    }

    if (childPathStr == parentPathStr)
    {
      result = "";
      return aww::Result::ok();
    }
    // subtract gitrepo from optionalPathAbsolute
    std::string relativeRepoUrlPath =
        childPathStr.substr(parentPathStr.length() + 1);

    // replace backslashes with forward slashes
    std::replace(relativeRepoUrlPath.begin(), relativeRepoUrlPath.end(), '\\', '/');
    result = relativeRepoUrlPath;
    return aww::Result::ok();
  }

  aww::Result try_find_repository_url_in_git_config(std::istream &gitConfigStream, std::string &gitSshOrHttpUri)
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
      return aww::Result::fail(
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
        std::cout << "urlConfigLine: " << urlConfigLine << "\n";
        break;
      }
    }
    if (!foundUrl)
    {
      return aww::Result::fail(
          "Configuration entry for remote origin url was not found");
    }
    gitSshOrHttpUri = urlConfigLine;
    return aww::Result::ok();
  }

  //  Attempts to convert git origin url to a web url
  //  git@github.com:dzharii/awwtools.git     => https://github.com/dzharii/awwtools.git
  //  https://github.com/dzharii/awwtools.git => https://github.com/dzharii/awwtools.git
  bool try_convert_to_git_url(const std::string &inputUrl, std::string &httpUrl)
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

      if (!std::regex_search(inputUrl, match, reGithubSsh))
      {
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

      if (!std::regex_search(inputUrl, match, reGithubSsh))
      {
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

  bool find_git_repo(const fs::path &dirPath, fs::path &gitRepoPath)
  {
    std::cout << "Searching for git repo in: " << dirPath << "\n";
    fs::path currentDir(dirPath);
    fs::path gitPath = currentDir / ".git";
    std::cout << "gitPath: " << gitPath << "\n";
    if (fs::exists(gitPath))
    {
      std::cout << "Found git repo in: " << currentDir << "\n";
      // The parent of the .git directory is the git repo
      gitRepoPath = gitPath.parent_path();
      return true;
    }

    std::cout << "No git repo found in: " << currentDir << "\n";
    const bool isRoot = currentDir == currentDir.root_path();
    if (!isRoot)
    {
      currentDir = currentDir.parent_path();
      return find_git_repo(currentDir, gitRepoPath);
    }
    std::cout << "Reached root directory, no git repo found."
              << "\n";
    return false;
  }
}

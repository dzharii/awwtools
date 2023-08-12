
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

    bool optionalFileToOpenExists = deps.fs_exists(optionalFileToOpen);

    if (optionalFileToOpenExists)
    {
      optionalPathAbsolute = deps.fs_get_absolute_path(optionalFileToOpen);

      // check if it is file or directory
      bool optionalPathIsDirectory = deps.fs_is_directory(optionalPathAbsolute);
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

    bool gitRepoFound = find_git_repo(currentDir, gitRepo, deps);

    if (!gitRepoFound)
    {
      std::cout << "No git repository found in " << currentDir << "\n";
      return 1;
    }

    std::cout << "Found git repo: " << gitRepo << "\n";
    fs::path gitConfigPath = gitRepo / ".git" / "config";
    std::cout << "git config path: " << gitConfigPath << "\n";

    // read line by line
    std::vector<std::string> gitConfigLines;
    if (aww::Result res = deps.fs_read_lines(gitConfigPath, gitConfigLines); res.is_failed())
    {
      std::cout << "Failed to read git config file"
                << res.error()
                << "\n";
      return 1;
    }


    std::string repoUrl;
    aww::Result findUrlResult = try_find_repository_url_in_git_config(gitConfigLines, repoUrl);
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
    fs::path gitRepoAbsolute = deps.fs_get_absolute_path(gitRepo);

    // TODO Error handling and logging

    if (!optionalPathAbsolute.empty())
    {
      std::string webPath;

      if (aww::Result res = get_relative_url_path(gitRepoAbsolute, optionalPathAbsolute, webPath); res.is_failed())
      {
        std::cout << "Failed to convert path to web url"
                  << res.error()
                  << "\n";
        return 1;
      }

      webUrl = webUrl + "?path=" + webPath;
      std::cout << "Opening file: " << webUrl << "\n";
    }

    if (aww::Result res = deps.launch_file_in_browser(webUrl); res.is_failed())
    {
      std::cout << "Failed to launch file" << res.error() << "\n";
      deps.show_notification("aww git open", "Failed to open file in browser");
      return 1;
    }
    std::cout << "Launched file\n";

    deps.show_notification("aww git open", "The file was opened in browser");
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

aww::Result try_find_repository_url_in_git_config(const std::vector<std::string>& gitConfigLines, std::string& gitSshOrHttpUri)
{
  // Find [remote "origin"] entry
  auto originIter = std::find(gitConfigLines.begin(), gitConfigLines.end(), "[remote \"origin\"]");
  if (originIter == gitConfigLines.end())
  {
    return aww::Result::fail("Configuration entry for remote origin was not found");
  }

  // Find url = entry
  auto urlIter = std::find_if(gitConfigLines.begin(), gitConfigLines.end(), [](const std::string& line) {
    return line.find("url = ") != std::string::npos;
  });

  if (urlIter == gitConfigLines.end())
  {
    return aww::Result::fail("Configuration entry for remote origin url was not found");
  }

  // Trim whitespace and extract URL
  const std::string& urlConfigLine = *urlIter;
  std::string urlValue = urlConfigLine.substr(urlConfigLine.find('=') + 1);

  urlValue = aww::string::trim(urlValue);

  gitSshOrHttpUri = urlValue;
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

  bool find_git_repo(const fs::path &dirPath, fs::path &gitRepoPath, aww_git_open_io_dependencies_interface &deps)
  {
    std::cout << "Searching for git repo in: " << dirPath << "\n";
    fs::path currentDir(dirPath);
    fs::path gitPath = currentDir / ".git";
    std::cout << "gitPath: " << gitPath << "\n";

    if (deps.fs_exists(gitPath))
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
      return find_git_repo(currentDir, gitRepoPath, deps);
    }
    std::cout << "Reached root directory, no git repo found."
              << "\n";
    return false;
  }
}

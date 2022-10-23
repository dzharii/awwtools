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
#include <regex>

#include "aww-common.hpp"

bool findGitRepo(std::filesystem::path, std::filesystem::path &);
bool tryConvertToGitUrl(std::string, std::string &);

/*
 * Simple main program that demonstrates how access
 * CMake definitions (here the version number) from source code.
 */
int main()
{
    std::cout << "Embrace the Aww!" << std::endl;

    std::filesystem::path currentDir = std::filesystem::current_path();
    std::filesystem::path gitRepo;

    if (findGitRepo(currentDir, gitRepo))
    {
        std::cout << "Found git repo: " << gitRepo << std::endl;
        auto gitConfigPath = gitRepo / "config";
        std::cout << "git config path: " << gitConfigPath << std::endl;

        // read line by line
        std::ifstream file(gitConfigPath);
        std::string str;

        bool foundOrigin = false;
        while (std::getline(file, str))
        {
            if (str == "[remote \"origin\"]")
            {
                foundOrigin = true;
                break;
            }
        }

        std::string urlConfigLine;
        bool foundUrl = false;

        if (foundOrigin)
        {
            while (std::getline(file, str))
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
        }
        else
        {
            std::cout << "No origin found" << std::endl;
        }

        if (foundUrl)
        {
            std::cout << "Found url: " << urlConfigLine << std::endl;

            std::string webUrl;
            if (tryConvertToGitUrl(urlConfigLine, webUrl))
            {
                std::cout << "Converted to web url: " << webUrl << std::endl;

                aww::result_t launchFileRes = aww::os::actions::launchFile(webUrl);
                if (aww::failed(launchFileRes))
                {
                    std::cout << aww::make_error("Failed to launch file", launchFileRes) << std::endl;
                }
                std::cout << "Launched file" << std::endl;
            }
            else
            {
                std::cout << "Failed to convert to web url" << std::endl;
            }
        }
        else
        {
            std::cout << "Could not find url" << std::endl;
        }
    }
    else
    {
        std::cout << "No git repo found." << std::endl;
    }

    return 0;
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
        gitRepoPath = gitPath.string();
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

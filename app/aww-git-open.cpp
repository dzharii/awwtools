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

bool findGitRepo(std::filesystem::path, std::filesystem::path &);

/*
 * Simple main program that demontrates how access
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


        if (foundOrigin)
        {
            while (std::getline(file, str))
            {
                if (str.find("url = ") != std::string::npos)
                {

                    // split str on =
                    std::string delimiter = "=";
                    urlConfigLine = str.substr(str.find(delimiter) + 1);
                    std::cout << "urlConfigLine: " << urlConfigLine << std::endl;

                    break;
                }
            }
        }
        else
        {
            std::cout << "No origin found" << std::endl;
        }
    }
    else
    {
        std::cout << "No git repo found." << std::endl;
    }

    return 0;
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
    else
    {
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
}
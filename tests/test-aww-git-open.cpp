#include "doctest/doctest.h"
#include <string>
#include <vector>
#include <filesystem>

#include "aww-common.hpp"
#include "internal/aww-git-open.hpp"

class aww_git_open_io_dependencies_stub : public aww::internal::aww_git_open::aww_git_open_io_dependencies_interface
{
public:
    std::function<std::filesystem::path(int, const std::filesystem::path &)>
        fs_get_absolute_path_stub = [this]([[maybe_unused]] int callCount, [[maybe_unused]] const std::filesystem::path &path) -> std::filesystem::path
    {
        return path;
    };
    int fs_get_absolute_path_called = 0;

    std::function<aww::Result(int, std::filesystem::path &)>
        fs_get_current_directory_absolute_path_stub = [this]([[maybe_unused]] int callCount, [[maybe_unused]] std::filesystem::path &result) -> aww::Result
    {
        result = "/";
        return aww::Result::ok();
    };
    int fs_get_current_directory_absolute_path_called = 0;

    std::function<bool(int, const std::filesystem::path &)>
        fs_exists_stub = [this]([[maybe_unused]] int callCount, [[maybe_unused]] const std::filesystem::path &target) -> bool
    {
        return true;
    };
    int fs_exists_called = 0;

    std::function<bool(int, const std::filesystem::path &)>
        fs_is_directory_stub = [this]([[maybe_unused]] int callCount, [[maybe_unused]] const std::filesystem::path &path) -> bool
    {
        return true;
    };
    int fs_is_directory_called = 0;

    std::function<aww::Result(int, const std::filesystem::path &, std::vector<std::string> &)>
        fs_read_lines_stub = [this]([[maybe_unused]] int callCount, [[maybe_unused]] const std::filesystem::path &filePath, [[maybe_unused]] std::vector<std::string> &outFileLines) -> aww::Result
    {
        outFileLines.push_back("url = https://github.com/user/repo.git");
        return aww::Result::ok();
    };
    int fs_read_lines_called = 0;

    std::filesystem::path fs_get_absolute_path(const std::filesystem::path &path) override
    {
        fs_get_absolute_path_called += 1;
        return fs_get_absolute_path_stub(fs_get_absolute_path_called, path);
    }

    aww::Result fs_get_current_directory_absolute_path(std::filesystem::path &result) override
    {
        fs_get_current_directory_absolute_path_called += 1;
        return fs_get_current_directory_absolute_path_stub(fs_get_current_directory_absolute_path_called, result);
    }

    bool fs_exists(const std::filesystem::path &target) override
    {
        fs_exists_called += 1;
        return fs_exists_stub(fs_exists_called, target);
    }

    bool fs_is_directory(const std::filesystem::path &path) override
    {
        fs_is_directory_called += 1;
        return fs_is_directory_stub(fs_is_directory_called, path);
    }

    aww::Result fs_read_lines(const std::filesystem::path &filePath, std::vector<std::string> &outFileLines) override
    {
        fs_read_lines_called += 1;
        return fs_read_lines_stub(fs_read_lines_called, filePath, outFileLines);
    }
};

TEST_CASE("try_convert_to_git_url") {
    aww_git_open_io_dependencies_stub deps;
    std::string inputUrl = "git@github.com:user/repo.git";
    std::string httpUrl;
    bool result = aww::internal::aww_git_open::try_convert_to_git_url(inputUrl, httpUrl);
    CHECK(result == true);
    CHECK(httpUrl == "https://github.com/user/repo.git");
}

/* TODO:
TEST_CASE("try_find_repository_url_in_git_config") {
    aww_git_open_io_dependencies_stub deps;
    std::vector<std::string> gitConfigLines = {"url = https://github.com/user/repo.git"};
    std::string gitSshOrHttpUri;
    aww::Result result = aww::internal::aww_git_open::try_find_repository_url_in_git_config(gitConfigLines, gitSshOrHttpUri);
    CHECK(result.is_ok());
    CHECK(gitSshOrHttpUri == "https://github.com/user/repo.git");
}
*/

/* TODO:
TEST_CASE("find_git_repo") {
    aww_git_open_io_dependencies_stub deps;
    fs::path dirPath = "/";
    fs::path gitRepoPath;
    bool result = aww::internal::aww_git_open::find_git_repo(dirPath, gitRepoPath, deps);
    CHECK(result == true);
}


TEST_CASE("get_relative_url_path") {
    aww_git_open_io_dependencies_stub deps;
    fs::path parentAbsPath = "/repo";
    fs::path childAbsPath = "/repo/subdir/file.txt";
    std::string result;
    aww::Result res = aww::internal::aww_git_open::get_relative_url_path(parentAbsPath, childAbsPath, result);
    CHECK(res.is_ok());
    CHECK(result == "subdir/file.txt");
}
*/


TEST_CASE("aww_git_open_main THIS TEST IS NOT IMPLEMENTED YET!!!!!!!!!!!!!") {
    aww_git_open_io_dependencies_stub deps;

    // TODO: continue with try_find_repository_url_in_git_config mocks

/*
    SUBCASE("No arguments provided") {
        std::vector<std::string> cmdArgs;
        int result = aww::internal::aww_git_open::aww_git_open_main(cmdArgs, deps);
        CHECK(result == 0);
    }

    SUBCASE("Too many arguments provided") {
        std::vector<std::string> cmdArgs = {"arg1", "arg2"};
        int result = aww::internal::aww_git_open::aww_git_open_main(cmdArgs, deps);
        CHECK(result == 1);
    }

    SUBCASE("Git repository found") {
        std::vector<std::string> cmdArgs = {"arg1"};
        int result = aww::internal::aww_git_open::aww_git_open_main(cmdArgs, deps);
        CHECK(result == 0);
    }

    SUBCASE("Git repository not found") {
        std::vector<std::string> cmdArgs = {"arg1"};
        int result = aww::internal::aww_git_open::aww_git_open_main(cmdArgs, deps);
        CHECK(result == 1);
    }

*/
}

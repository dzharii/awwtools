#include "doctest/doctest.h"
#include <string>
#include <vector>
#include <filesystem>

#include "aww-common.hpp"
#include "internal/aww-git-open.hpp"

class aww_git_open_io_dependencies_stub : public aww::internal::aww_git_open::aww_git_open_io_dependencies_interface
{
public:
    // fs_get_absolute_path_stub
    std::function<std::filesystem::path(int, const std::filesystem::path &)>
        fs_get_absolute_path_stub = [this]([[maybe_unused]] int callCount, [[maybe_unused]] const std::filesystem::path &path) -> std::filesystem::path
    {
        return path;
    };

    int fs_get_absolute_path_called = 0;

    std::filesystem::path fs_get_absolute_path(const std::filesystem::path &path) override
    {
        fs_get_absolute_path_called += 1;
        return fs_get_absolute_path_stub(fs_get_absolute_path_called, path);
    }

    // fs_get_current_directory_absolute_path_stub
    std::function<aww::Result(int, std::filesystem::path &)>
        fs_get_current_directory_absolute_path_stub = [this]([[maybe_unused]] int callCount, [[maybe_unused]] std::filesystem::path &result) -> aww::Result
    {
        result = "/";
        return aww::Result::ok();
    };

    int fs_get_current_directory_absolute_path_called = 0;

    aww::Result fs_get_current_directory_absolute_path(std::filesystem::path &result) override
    {
        fs_get_current_directory_absolute_path_called += 1;
        return fs_get_current_directory_absolute_path_stub(fs_get_current_directory_absolute_path_called, result);
    }

    // fs_exists_stub
    std::function<bool(int, const std::filesystem::path &)>
        fs_exists_stub = [this]([[maybe_unused]] int callCount, [[maybe_unused]] const std::filesystem::path &target) -> bool
    {
        return true;
    };

    int fs_exists_called = 0;

    bool fs_exists(const std::filesystem::path &target) override
    {
        fs_exists_called += 1;
        return fs_exists_stub(fs_exists_called, target);
    }

    // fs_is_directory_stub
    std::function<bool(int, const std::filesystem::path &)>
        fs_is_directory_stub = [this]([[maybe_unused]] int callCount, [[maybe_unused]] const std::filesystem::path &path) -> bool
    {
        return true;
    };

    int fs_is_directory_called = 0;

    bool fs_is_directory(const std::filesystem::path &path) override
    {
        fs_is_directory_called += 1;
        return fs_is_directory_stub(fs_is_directory_called, path);
    }

    // fs_read_lines_stub
    std::function<aww::Result(int, const std::filesystem::path &, std::vector<std::string> &)>
        fs_read_lines_stub = [this]([[maybe_unused]] int callCount, [[maybe_unused]] const std::filesystem::path &filePath, [[maybe_unused]] std::vector<std::string> &outFileLines) -> aww::Result
    {
        outFileLines.push_back("url = https://github.com/user/repo.git");
        return aww::Result::ok();
    };

    int fs_read_lines_called = 0;

    aww::Result fs_read_lines(const std::filesystem::path &filePath, std::vector<std::string> &outFileLines) override
    {
        fs_read_lines_called += 1;
        return fs_read_lines_stub(fs_read_lines_called, filePath, outFileLines);
    }

    // launch_file_in_browser_stub
    std::function<aww::Result(const std::string&)>
        launch_file_in_browser_stub = [this]([[maybe_unused]]const std::string &url) -> aww::Result
    {
        return aww::Result::ok();
    };
    int launch_file_in_browser_called = 0;

    aww::Result launch_file_in_browser(const std::string &url) override
    {
        launch_file_in_browser_called += 1;
        return launch_file_in_browser_stub(url);
    }

    // show_notification_stub
    std::function<void(const std::string&, const std::string&)>
        show_notification_stub = [this]([[maybe_unused]]const std::string &title, [[maybe_unused]]const std::string &message)
    {
        // empty
    };

    int show_notification_called = 0;

    void show_notification(const std::string &title, const std::string &message) override
    {
        show_notification_called += 1;
        show_notification_stub(title, message);
    }
};

TEST_CASE("try_convert_to_git_url accepatance tests")
{
    aww_git_open_io_dependencies_stub deps;

    const std::string input_url_SSH_github = "git@github.com:user/repo.git";
    const std::string input_url_HTTPS_github = "https://github.com/apache/arrow";
    const std::string input_url_SSH_azure_devops = "example@vs-ssh.visualstudio.com:v3/example/ProjectName/RepoName";
    const std::string input_url_HTTPS_azure_devops = "https://example.visualstudio.com/DefaultCollection/ProjectName/_git/RepoName";

    SUBCASE("try_convert_to_git_url: input_url_SSH_github")
    {
        std::string httpUrl;
        bool result = aww::internal::aww_git_open::try_convert_to_git_url(input_url_SSH_github, httpUrl);
        CHECK(result == true);
        CHECK(httpUrl == "https://github.com/user/repo.git");
    }

    SUBCASE("try_convert_to_git_url: input_url_HTTPS_github")
    {
        std::string httpUrl;
        bool result = aww::internal::aww_git_open::try_convert_to_git_url(input_url_HTTPS_github, httpUrl);
        CHECK(result == true);
        CHECK(httpUrl == "https://github.com/apache/arrow");
    }

    SUBCASE("try_convert_to_git_url: input_url_SSH_azure_devops")
    {
        std::string httpUrl;
        bool result = aww::internal::aww_git_open::try_convert_to_git_url(input_url_SSH_azure_devops, httpUrl);
        CHECK(result == true);
        CHECK(httpUrl == "https://example.visualstudio.com/DefaultCollection/ProjectName/_git/RepoName");
    }

    SUBCASE("try_convert_to_git_url: input_url_HTTPS_azure_devops")
    {
        std::string httpUrl;
        bool result = aww::internal::aww_git_open::try_convert_to_git_url(input_url_HTTPS_azure_devops, httpUrl);
        CHECK(result == true);
        CHECK(httpUrl == "https://example.visualstudio.com/DefaultCollection/ProjectName/_git/RepoName");
    }
}

TEST_CASE("try_find_repository_url_in_git_config accepatance tests")
{
    aww_git_open_io_dependencies_stub deps;
    std::vector<std::string> gitConfigLines = {
        "[remote \"origin\"]",
        "    url = https://github.com/user/repo.git"};

    std::string gitSshOrHttpUri;
    aww::Result result = aww::internal::aww_git_open::try_find_repository_url_in_git_config(gitConfigLines, gitSshOrHttpUri);
    CHECK(result.is_ok());
    CHECK(result.error() == "");

    CHECK(gitSshOrHttpUri == "https://github.com/user/repo.git");
}

TEST_CASE("find_git_repo accepatance tests")
{
    aww_git_open_io_dependencies_stub deps;

    SUBCASE("when executed from a valid nested folder of git repo, it should return the root git repo path") {
        const std::filesystem::path root = "/";
        const std::filesystem::path dirPath = root / "Repo" / "A" / "B" / "C";
        std::filesystem::path gitRepoPath;

        deps.fs_exists_stub = [&]([[maybe_unused]] int callCount, [[maybe_unused]] const std::filesystem::path &target) -> bool
        {
            const std::filesystem::path gitRepoRoot = root / "Repo" / ".git";
            if (target == gitRepoRoot)
            {
                return true;
            }

            return false;
        };

        bool result = aww::internal::aww_git_open::find_git_repo(dirPath, gitRepoPath, deps);

        std::filesystem::path expectedPath = root / "Repo";

        CHECK(result == true);
        CHECK(gitRepoPath ==  expectedPath);
    }

    SUBCASE("when executed from a valid root folder of git repo, it should return the root git repo path") {
        const std::filesystem::path root = "/";
        const std::filesystem::path dirPath = root / "Repo";
        std::filesystem::path gitRepoPath;

        deps.fs_exists_stub = [&]([[maybe_unused]] int callCount, [[maybe_unused]] const std::filesystem::path &target) -> bool
        {
            const std::filesystem::path gitRepoRoot = root / "Repo" / ".git";
            if (target == gitRepoRoot)
            {
                return true;
            }

            return false;
        };

        bool result = aww::internal::aww_git_open::find_git_repo(dirPath, gitRepoPath, deps);
        CHECK(result == true);
        CHECK(gitRepoPath == root / "Repo");
    }

    SUBCASE("should fail when there is no .git folder in the path") {
        const std::filesystem::path dirPath = "C:\\Repo\\A\\B\\C";
        std::filesystem::path gitRepoPath;

        deps.fs_exists_stub = []([[maybe_unused]] int callCount, [[maybe_unused]] const std::filesystem::path &target) -> bool
        {
            return false;
        };

        bool result = aww::internal::aww_git_open::find_git_repo(dirPath, gitRepoPath, deps);
        CHECK(result == false);
    }
}

TEST_CASE("get_relative_url_path acceptance tests") {
    aww_git_open_io_dependencies_stub deps;
    std::filesystem::path parentAbsPath = "/repo";
    std::filesystem::path childAbsPath = "/repo/subdir/file.txt";
    std::string result;
    aww::Result res = aww::internal::aww_git_open::get_relative_url_path(parentAbsPath, childAbsPath, result);
    CHECK(res.is_ok());
    CHECK(result == "subdir/file.txt");
}

TEST_CASE("aww_git_open_main THIS TEST IS NOT IMPLEMENTED YET!!!!!!!!!!!!!")
{
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

#include "aww-common.hpp"
#include <fstream>
#include <algorithm>
#include <cctype>

namespace aww::date
{
  // TODO: - 2022-10-18 [Exploring C++11, part 2 localtime and time again Kjellkod's Blog](https://kjellkod.wordpress.com/2013/01/22/exploring-c11-part-2-localtime-and-time-again/)
  std::string get_date_YYYYMMDD(void)
  {
    std::string date;
    char buff[32]{};
    time_t t = time(nullptr);

    if (std::strftime(buff, sizeof(buff), "%F", std::localtime(&t)))
    {
      date = buff;
    }
    return date;
  }
}

namespace aww::string
{
  std::string join(const std::vector<std::string> &v, const std::string &delimiter)
  {
    // Glen Knowles - https://stackoverflow.com/a/40497964
    std::string out;
    if (auto i = v.begin(), e = v.end(); i != e)
    {
      out += *i++;
      for (; i != e; ++i)
        out.append(delimiter).append(*i);
    }
    return out;
  }

  std::string left_padding(const std::string &inp, const char &padChar, const size_t &padLength)
  {
    const size_t inpLength = inp.length();
    if (inpLength >= padLength)
    {
      return inp;
    }
    const size_t paddingLength = padLength - inpLength;
    std::string padding(paddingLength, padChar);
    return padding + inp;
  }

  std::string to_upper(const std::string &inp)
  {
    std::string out(inp);
    for (char &c : out)
    {
      c = static_cast<char>(std::toupper(c));
    }
    return out;
  }

  std::string to_lower(const std::string &inp)
  {
    std::string out(inp);
    for (char &c : out)
    {
      c = static_cast<char>(std::tolower(c));
    }
    return out;
  }

  bool ends_with(const std::string &str, const std::string &suffix)
  {
    if (suffix.size() > str.size())
    {
      return false;
    }
    return std::equal(suffix.rbegin(), suffix.rend(), str.rbegin());
  }

  std::string to_valid_identifier(const std::string &input)
  {
    std::string output = input;
    std::replace_if(
        output.begin(), output.end(), [](char c)
        { return !std::isalnum(c); },
        '_');
    return output;
  }

  std::string trim(std::string str)
  {
    // Trim leading whitespace
    str.erase(str.begin(), std::find_if(str.begin(), str.end(), [](int ch)
                                        { return !std::isspace(ch); }));

    // Trim trailing whitespace
    str.erase(std::find_if(str.rbegin(), str.rend(), [](int ch)
                           { return !std::isspace(ch); })
                  .base(),
              str.end());

    return str;
  }
}

namespace aww::os
{
  Proccess::Proccess()
  {
    this->onStdOutCallback = defaultStdOutCallback;
    this->onStdErrCallback = defaultStdErrCallback;
    this->onExitCallback = defaultExitCallback;
  }

  Proccess &Proccess::onStdOut(std::function<void(const std::string)> callback)
  {
    onStdOutCallback = callback;
    return *this;
  }

  Proccess &Proccess::onStdErr(std::function<void(const std::string)> callback)
  {
    onStdErrCallback = callback;
    return *this;
  }

  Proccess &Proccess::onExit(std::function<void(int)> callback)
  {
    onExitCallback = callback;
    return *this;
  }

  int Proccess::run(const std::string &cmd)
  {
    FILE *fp = popen(cmd.c_str(), "r");
    if (fp == nullptr)
    {
      throw std::runtime_error("Could not open pipe");
    }
    const size_t bufferSize = 1024;
    char buffer[bufferSize];
    while (fgets(buffer, bufferSize, fp) != nullptr)
    {
      onStdOutCallback(buffer);
    }
    int exitCode = pclose(fp);
    onExitCallback(exitCode);
    return exitCode;
  }

  void Proccess::defaultStdOutCallback(std::string)
  {
  }

  void Proccess::defaultStdErrCallback(std::string)
  {
  }

  void Proccess::defaultExitCallback(int)
  {
  }
}

namespace aww::os::env
{
  aww::Result get_user_home_dir(std::filesystem::path &outHomeDir)
  {
    char *homeDir = nullptr;

    homeDir = std::getenv("HOME");
    if (homeDir != nullptr)
    {
      outHomeDir = std::filesystem::absolute(homeDir);
      return aww::Result::ok();
    }

    homeDir = std::getenv("USERPROFILE");
    if (homeDir != nullptr)
    {
      outHomeDir = std::filesystem::absolute(homeDir);
      return aww::Result::ok();
    }
    return aww::Result::fail("Could not find user home directory");
  }

  std::filesystem::path get_aww_dot_dir(void)
  {
    std::filesystem::path homeDir;
    aww::Result result = get_user_home_dir(homeDir);
    if (result.is_failed())
    {
      return std::filesystem::path(); // empty path
    }
    return homeDir / ".awwtools";
  }
}

namespace aww::fs
{
  aww::Result get_current_directory_absolute_path(std::filesystem::path &result)
  {
    try
    {
      result = std::filesystem::current_path();
      result = std::filesystem::absolute(result);
      return aww::Result::ok();
    }
    catch (const std::exception &e)
    {
      std::string errorMessage = "Error getting current directory: " + std::string(e.what());
      return aww::Result::fail(errorMessage);
    }
    catch (...)
    {
      std::string errorMessage = "Unknown error occurred while getting current directory.";
      return aww::Result::fail(errorMessage);
    }
  }

  aww::Result is_directory(const std::filesystem::path &path, bool &outIsDirectory)
  {
    try
    {
      outIsDirectory = std::filesystem::is_directory(path);
      return aww::Result::ok();
    }
    catch (const std::exception &e)
    {
      std::string errorMessage = "Error checking if directory: " + std::string(e.what());
      return aww::Result::fail(errorMessage);
    }
    catch (...)
    {
      std::string errorMessage = "Unknown error occurred while checking if directory.";
      return aww::Result::fail(errorMessage);
    }
  }

  aww::Result create_empty_file(const std::filesystem::path &path)
  {
    try
    {
      std::ofstream file(path);
      file.close();
      return aww::Result::ok();
    }
    catch (const std::exception &e)
    {
      std::string errorMessage = "Error creating file '" + path.filename().string() + "': " + e.what();
      return aww::Result::fail(errorMessage);
    }
    catch (...)
    {
      std::string errorMessage = "Unknown error occurred while creating file '" + path.filename().string() + "'.";
      return aww::Result::fail(errorMessage);
    }
  }

  aww::Result read_lines(const std::filesystem::path &filePath, std::vector<std::string> &outFileLines)
  {
    try
    {
      std::ifstream file(filePath);
      if (!file.is_open())
      {
        return aww::Result::fail("Failed to open file for reading.");
      }

      std::string line;
      while (std::getline(file, line))
      {
        outFileLines.push_back(line);
      }

      file.close();
      return aww::Result::ok();
    }
    catch (const std::exception &e)
    {
      std::string errorMessage = "Error reading file: " + std::string(e.what());
      return aww::Result::fail(errorMessage);
    }
    catch (...)
    {
      std::string errorMessage = "Unknown error occurred while reading file.";
      return aww::Result::fail(errorMessage);
    }
  }

  aww::Result write_lines(const std::filesystem::path &filePath, const std::vector<std::string> &lines)
  {
    try
    {
      std::ofstream file(filePath);
      if (!file.is_open())
      {
        return aww::Result::fail("Failed to open file for writing.");
      }

      for (const auto &line : lines)
      {
        file << line << std::endl;
      }

      file.close();
      return aww::Result::ok();
    }
    catch (const std::exception &e)
    {
      std::string errorMessage = "Error writing file: " + std::string(e.what());
      return aww::Result::fail(errorMessage);
    }
    catch (...)
    {
      std::string errorMessage = "Unknown error occurred while writing file.";
      return aww::Result::fail(errorMessage);
    }
  }

  std::string read_ascii_text_file(const std::filesystem::path &path)
  {
    std::ifstream file(path);
    std::string content((std::istreambuf_iterator<char>(file)), std::istreambuf_iterator<char>());
    return content;
  }

  std::string normalize_file_path(const std::string &inputPath)
  {
    std::filesystem::path path(inputPath);
    std::filesystem::path canonicalPath = std::filesystem::weakly_canonical(path);
    std::string normalizedPath = canonicalPath.make_preferred().string();
    return normalizedPath;
  }
}

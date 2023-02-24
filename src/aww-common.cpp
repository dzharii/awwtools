#include "aww-common.hpp"
#include <fstream>
#include <algorithm>
#include <cctype>

namespace aww::date
{
  // TODO: - 2022-10-18 [Exploring C++11, part 2 localtime and time again Kjellkod's Blog](https://kjellkod.wordpress.com/2013/01/22/exploring-c11-part-2-localtime-and-time-again/)
  std::string getDateYYYYMMDD(void)
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

  std::string leftPadding(const std::string &inp, const char &padChar, const size_t &padLength)
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

  std::string toupper(const std::string &inp)
  {
    std::string out(inp);
    for (char &c : out)
    {
      c = static_cast<char>(std::toupper(c));
    }
    return out;
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

  Proccess& Proccess::onStdOut(std::function<void(const std::string)> callback)
  {
    onStdOutCallback = callback;
    return *this;
  }

  Proccess& Proccess::onStdErr(std::function<void(const std::string)> callback)
  {
    onStdErrCallback = callback;
    return *this;
  }

  Proccess& Proccess::onExit(std::function<void(int)> callback)
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
  aww::Result getUserHomeDir(std::filesystem::path& outHomeDir)
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

  std::filesystem::path getAwwDotDir(void)
  {
    std::filesystem::path homeDir;
    aww::Result result = getUserHomeDir(homeDir);
    if (result.isFailed())
    {
      return std::filesystem::path(); // empty path
    }
    return homeDir / ".awwtools";
  }
}

namespace aww::fs {

  std::string readAsciiTextFile(const std::filesystem::path& path)
  {
    std::ifstream file(path);
    std::string content((std::istreambuf_iterator<char>(file)), std::istreambuf_iterator<char>());
    return content;
  }

  std::string normalizeFilePath(const std::string& inputPath)
  {
    std::filesystem::path path(inputPath);
    std::filesystem::path canonicalPath = std::filesystem::weakly_canonical(path);
    std::string normalizedPath = canonicalPath.make_preferred().string();
    return normalizedPath;
  }
}

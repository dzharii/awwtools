#include "aww-common.hpp"

namespace aww {

  bool failed(const aww::result_t& result)
  {
    return !std::get<aww::resultPos>(result);
  }

  bool succeeded(const aww::result_t& result)
  {
    return std::get<aww::resultPos>(result);
  }

  std::string make_error(const std::string& description, const aww::result_t& result)
  {
    std::string msg = description + ": " + std::get<aww::errorPos>(result);
    return msg;
  }
}

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

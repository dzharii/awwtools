#pragma once
#ifndef AWW_RUN_HPP
#define AWW_RUN_HPP

#include <filesystem>
#include <string>
#include "aww-common.hpp"

namespace aww::internal::aww_run
{
  namespace fs = std::filesystem;

  aww::Result findScriptWindows(const std::string &, fs::path &);
  aww::Result findScriptLinux(const std::string &, fs::path &);
  aww::Result findScriptMacOS(const std::string &, fs::path &);
  aww::Result findScript(const std::string &, fs::path &);

  int aww_run_main(int, char **);
}

#endif // AWW_RUN_HPP

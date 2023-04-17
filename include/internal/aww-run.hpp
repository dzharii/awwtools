#pragma once
#ifndef AWW_RUN_HPP
#define AWW_RUN_HPP

#include <filesystem>
#include <vector>
#include <string>
#include "aww-common.hpp"

namespace aww::internal::aww_run
{
  namespace fs = std::filesystem;

  aww::Result find_script_windows(const std::string &, fs::path &);
  aww::Result find_script_linux(const std::string &, fs::path &);
  aww::Result find_script(const std::string &, fs::path &);

  int aww_run_main(const std::vector<std::string> &cmdArgs);
}

#endif // AWW_RUN_HPP

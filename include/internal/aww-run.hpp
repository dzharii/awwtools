#pragma once
#ifndef AWW_RUN_HPP
#define AWW_RUN_HPP

#include "aww-common.hpp"
#include <filesystem>
#include <string>
#include <vector>

namespace aww::internal::aww_run {
namespace fs = std::filesystem;

aww::Result find_script_windows(const std::string&, fs::path&);
aww::Result find_script_linux(const std::string&, fs::path&);
aww::Result find_script(const std::string&, fs::path&);

int aww_run_main(const std::vector<std::string>& cmdArgs);
} // namespace aww::internal::aww_run

#endif // AWW_RUN_HPP

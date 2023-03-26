#pragma once
#ifndef AWW_CREATE_HPP
#define AWW_CREATE_HPP

#include <filesystem>

#include "aww-common.hpp"

namespace aww::internal::aww_create
{
  namespace fs = std::filesystem;

  aww::Result try_create_file_by_path(const fs::path &);
  int aww_create_main(int, char **);
}

#endif // AWW_CREATE_HPP

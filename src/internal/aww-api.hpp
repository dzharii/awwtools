#pragma once
#ifndef AWW_API_HPP
#define AWW_API_HPP

#include <string>
#include <vector>

#include "aww-common.hpp"
#include "clip.h"

namespace aww::internal::aww_api {

class aww_api_io_dependencies_interface {
public:
  virtual ~aww_api_io_dependencies_interface() = default;

  virtual void show_notification(const std::string& title, const std::string& message) = 0;
};

class aww_api_io_dependencies : public aww_api_io_dependencies_interface {
public:
  void show_notification(const std::string& title, const std::string& message) override {
    aww::os::actions::show_notification(title, message);
  }
};

int aww_api_main(const std::vector<std::string>& cmdArgs, aww_api_io_dependencies_interface& deps);
} // namespace aww::internal::aww_api

#endif

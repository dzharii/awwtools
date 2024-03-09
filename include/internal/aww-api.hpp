#pragma once
#ifndef AWW_API_HPP
#define AWW_API_HPP

#include <vector>
#include <string>

#include "clip.h"
#include "aww-common.hpp"


namespace aww::internal::aww_api {

  class aww_api_io_dependencies_interface {
    public:
      virtual ~aww_api_io_dependencies_interface() = default;

      virtual void show_notification(const std::string &title, const std::string &message, [[maybe_unused]] aww::call_tag_t tag) = 0;
  };

  class aww_api_io_dependencies : public aww_api_io_dependencies_interface {
    public:

      void show_notification(const std::string &title, const std::string &message, [[maybe_unused]] aww::call_tag_t tag) override {
        aww::os::actions::show_notification(title, message);
      }
  };

  int aww_api_main(const std::vector<std::string> &cmdArgs, aww_api_io_dependencies_interface &deps);
}

#endif

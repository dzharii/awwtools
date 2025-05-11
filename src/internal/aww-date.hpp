#pragma once
#ifndef AWW_DATE_HPP
#define AWW_DATE_HPP

#include <string>
#include <vector>

#include "aww-common.hpp"
#include "clip.h"

namespace aww::internal::aww_date {

class aww_date_io_dependencies_interface {
public:
  virtual ~aww_date_io_dependencies_interface() = default;

  virtual std::string get_date_yyyymmdd() = 0;
  virtual bool clipboard_set_text(const std::string& text) = 0;
  virtual void show_notification(const std::string& title, const std::string& message) = 0;
};

class aww_date_io_dependencies : public aww_date_io_dependencies_interface {
public:
  std::string get_date_yyyymmdd() override {
    return aww::date::get_date_YYYYMMDD();
  }

  bool clipboard_set_text(const std::string& text) override {
    return clip::set_text(text);
  }

  void show_notification(const std::string& title, const std::string& message) override {
    aww::os::actions::show_notification(title, message);
  }
};

int aww_date_main(const std::vector<std::string>& cmdArgs, aww_date_io_dependencies_interface& deps);
} // namespace aww::internal::aww_date

#endif

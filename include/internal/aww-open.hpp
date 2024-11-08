#pragma once
#ifndef AWW_OPEN_HPP
#define AWW_OPEN_HPP

#include <string>
#include <vector>

#include "aww-common.hpp"

namespace aww::internal::aww_open {

class aww_open_io_dependencies_interface {
public:
  virtual ~aww_open_io_dependencies_interface() = default;

  /**
   * Launch a file using the default program associated with its type.
   * @param filePath The path of the file to open.
   * @return Aww::Result indicating the success or failure of the operation.
   */
  virtual aww::Result launch_file(const std::string& filePath) = 0;

  /**
   * Show a notification.
   * @param title The title of the notification.
   * @param message The message of the notification.
   */
  virtual void show_notification(const std::string& title, const std::string& message) = 0;
};

class aww_open_io_dependencies : public aww_open_io_dependencies_interface {
public:
  aww::Result launch_file(const std::string& filePath) override {
    return aww::os::actions::launch_file(filePath);
  }

  void show_notification(const std::string& title, const std::string& message) override {
    aww::os::actions::show_notification(title, message);
  }
};

int aww_open_main(const std::vector<std::string>& cmdArgs,
                  aww_open_io_dependencies_interface& deps);
} // namespace aww::internal::aww_open

#endif // AWW_OPEN_HPP

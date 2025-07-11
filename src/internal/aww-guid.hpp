#pragma once
#ifndef AWW_GUID_HPP
#define AWW_GUID_HPP

#include "aww-result/aww-result.hpp"
#include <string>
#include <vector>

#include "aww-common.hpp"
#include "clip.h"

namespace aww::internal::aww_guid {
class aww_guid_io_dependencies_interface {
public:
  virtual ~aww_guid_io_dependencies_interface() = default;

  /**
   * Set the clipboard text with the provided text.
   * @param text The text to be set in the clipboard.
   * @return True if the operation was successful, false otherwise.
   */
  virtual bool clipboard_set_text(const std::string& text) = 0;

  /**
   * Show a notification.
   * @param title The title of the notification.
   * @param message The message of the notification.
   */
  virtual void show_notification(const std::string& title, const std::string& message) = 0;

  /**
   * @brief Get a GUID (Globally Unique Identifier) as a string. (aww tag #r1g7g8d2k1)
   * @return aww::result<std::string> containing the GUID or an error.
   */
  virtual aww::result<std::string> os_get_guid() = 0;
};

class aww_guid_io_dependencies : public aww_guid_io_dependencies_interface {
public:
  bool clipboard_set_text(const std::string& text) override {
    return clip::set_text(text);
  }

  void show_notification(const std::string& title, const std::string& message) override {
    aww::os::actions::show_notification(title, message);
  }

  aww::result<std::string> os_get_guid() override {
    // Use awwlib or fallback to old util if needed
    // Example fallback:
    std::string guid;
    if (auto legacy = aww::util::get_guid(guid); legacy.is_failed()) {
      return aww::result<std::string>::err("Failed to generate GUID");
    } else {
      return aww::result<std::string>::ok(std::move(guid));
    }
  }
};

int aww_guid_main(const std::vector<std::string>& cmdArgs, aww_guid_io_dependencies_interface& deps);
} // namespace aww::internal::aww_guid

#endif // AWW_GUID_HPP

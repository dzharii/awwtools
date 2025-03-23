#pragma once
#ifndef AWW_GUID_HPP
#define AWW_GUID_HPP

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
   * Get a GUID (Globally Unique Identifier) as a string.
   * @param out A reference to a string where the GUID will be stored.
   * @return Aww::Result indicating the success or failure of the operation.
   */
  virtual aww::Result os_get_guid(std::string& out) = 0;
};

class aww_guid_io_dependencies : public aww_guid_io_dependencies_interface {
public:
  bool clipboard_set_text(const std::string& text) override { return clip::set_text(text); }

  void show_notification(const std::string& title, const std::string& message) override {
    aww::os::actions::show_notification(title, message);
  }

  aww::Result os_get_guid(std::string& out) override { return aww::util::get_guid(out); }
};

int aww_guid_main(const std::vector<std::string>& cmdArgs,
                  aww_guid_io_dependencies_interface& deps);
} // namespace aww::internal::aww_guid

#endif // AWW_GUID_HPP

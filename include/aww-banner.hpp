#pragma once
#ifndef AWW_BANNER_HPP
#define AWW_BANNER_HPP

#include <string>
#include "aww-common.hpp"

namespace aww::banner
{
  class NotificationDialogBox
  {
  public:
    NotificationDialogBox(
        const std::string &title,
        const std::string &message,
        const aww::draw::RgbaColor &titleBackgroundColor,
        const aww::draw::RgbaColor &titleForegroundColor,
        int delayInSeconds)
        : title(title),
          message(message),
          titleBackgroundColor(titleBackgroundColor),
          titleForegroundColor(titleForegroundColor),
          delayInSeconds(delayInSeconds)
    {
    }
    void show();

    std::string getTitle() const
    {
      return title;
    }

    std::string getMessage() const
    {
      return message;
    }

  private:
    std::string title;
    std::string message;
    aww::draw::RgbaColor titleBackgroundColor;
    aww::draw::RgbaColor titleForegroundColor;
    int delayInSeconds;
  };
} // namespace aww::banner

#endif // AWW_BANNER_HPP

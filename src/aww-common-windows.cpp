#include <filesystem>

#include "aww-common.hpp"
#include "Windows.h"
#include "wintoastlib.h"

namespace aww::os::actions
{

  aww::result_t launchFile(const std::string &path)
  {
    // check path is null
    if (path.empty())
    {
      return std::make_tuple(false, "Argument path is empty");
    }
    HINSTANCE execResult = ShellExecuteA(nullptr, "open", path.c_str(), nullptr, nullptr, SW_SHOWNORMAL);
    const HINSTANCE Success = reinterpret_cast<HINSTANCE>(33);

    if (execResult >= Success)
    {
      return std::make_tuple(true, "");
    }

    unsigned long errorCode = GetLastError();

    if (errorCode == 0)
    {
      return std::make_tuple(false, "ShellExecuteA failed: The operating system is out of memory or resources.");
    }
    else if (errorCode == ERROR_FILE_NOT_FOUND)
    {
      return std::make_tuple(false, "ShellExecuteA failed: The specified file was not found.");
    }
    else if (errorCode == ERROR_PATH_NOT_FOUND)
    {
      return std::make_tuple(false, "ShellExecuteA failed: The specified path was not found.");
    }
    else if (errorCode == ERROR_BAD_FORMAT)
    {
      return std::make_tuple(false, "ShellExecuteA failed: The .exe file is invalid (non-Win32 .exe or error in .exe image).");
    }
    else if (errorCode == SE_ERR_ACCESSDENIED)
    {
      return std::make_tuple(false, "ShellExecuteA failed: The operating system denied access to the specified file.");
    }
    else if (errorCode == SE_ERR_ASSOCINCOMPLETE)
    {
      return std::make_tuple(false, "ShellExecuteA failed: The file name association is incomplete or invalid.");
    }
    else if (errorCode == SE_ERR_DDEBUSY)
    {
      return std::make_tuple(false, "ShellExecuteA failed: The DDE transaction could not be completed because other DDE transactions were being processed.");
    }
    else if (errorCode == SE_ERR_DDEFAIL)
    {
      return std::make_tuple(false, "ShellExecuteA failed: The DDE transaction failed.");
    }
    else if (errorCode == SE_ERR_DDETIMEOUT)
    {
      return std::make_tuple(false, "ShellExecuteA failed: The DDE transaction could not be completed because the request timed out.");
    }
    else if (errorCode == SE_ERR_DLLNOTFOUND)
    {
      return std::make_tuple(false, "ShellExecuteA failed: The specified DLL was not found.");
    }
    else if (errorCode == SE_ERR_NOASSOC)
    {
      return std::make_tuple(false, "ShellExecuteA failed: There is no application associated with the given file name extension.");
    }
    else if (errorCode == SE_ERR_OOM)
    {
      return std::make_tuple(false, "ShellExecuteA failed: There was not enough memory to complete the operation.");
    }
    else if (errorCode == SE_ERR_SHARE)
    {
      return std::make_tuple(false, "ShellExecuteA failed: A sharing violation occurred.");
    }
    return std::make_tuple(false, "ShellExecuteA failed: Unknown error.");
  }

  class WinToastHandlerExample : public WinToastLib::IWinToastHandler
  {
  public:
    WinToastHandlerExample() = default;
    // Public interfaces
    void toastActivated() const {

    };

    void toastActivated(int) const {

    }

    void toastDismissed(WinToastDismissalReason) const {

    };
    void toastFailed() const {

    };
  };

  static WinToastHandlerExample _wintoastDefaultHandler;

  aww::result_t showNotification(const std::string &title, const std::string &message)
  {
    // check title is null
    if (title.empty())
    {
      return std::make_tuple(false, "Argument title is empty");
    }
    // check message is null
    if (message.empty())
    {
      return std::make_tuple(false, "Argument message is empty");
    }

    WinToastLib::WinToastTemplate templ =
        WinToastLib::WinToastTemplate(WinToastLib::WinToastTemplate::Text02);

    templ.setTextField(
        L"Do you think this feature is cool?",
        WinToastLib::WinToastTemplate::FirstLine);

    templ.setTextField(
        L"Ofc,it is!",
        WinToastLib::WinToastTemplate::SecondLine);

    WinToastLib::WinToast::instance()->showToast(templ, &_wintoastDefaultHandler) ? 0 : 1;

    return std::make_tuple(true, "");
  }

}

namespace aww::fs
{
  std::filesystem::path getCurrentExecutablePath(void)
  {
    char buffer[MAX_PATH];
    DWORD nchar;
    nchar = GetModuleFileNameA(nullptr, buffer, MAX_PATH);
    if (nchar == 0)
    {
      return std::filesystem::path("");
    }
    return std::filesystem::path(buffer);
  }
}

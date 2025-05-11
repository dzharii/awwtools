#include <algorithm>
#include <cctype>
#include <codecvt>
#include <filesystem>
#include <locale>
#include <sstream>
#include <string>

#include "Windows.h"
#include "aww-common.hpp"
#include "wintoastlib.h"

namespace aww::os {
/**
 * @brief Checks if file is executable. (aww tag #20250511d1)
 */
bool can_execute(const std::filesystem::path& path) {
  const std::string fileExtension = path.extension().string();
  // fileExtension to lower case

  const bool isExecutable =
      std::filesystem::exists(path) &&
      (fileExtension == ".exe" || fileExtension == ".EXE" || fileExtension == ".bat" || fileExtension == ".BAT" ||
       fileExtension == ".cmd" || fileExtension == ".CMD" || fileExtension == ".ps1" || fileExtension == ".PS1");
  return isExecutable;
}

/**
 * @brief Gets command line args. (aww tag #20250511d2)
 */
std::vector<std::string> get_command_line_args(void) {
  int argc;
  LPWSTR* argv = CommandLineToArgvW(GetCommandLineW(), &argc);

  // Allocate a buffer to hold the converted strings
  const int bufferSize = 1024;
  char buffer[bufferSize];

  // Convert each Unicode string to a std::string
  std::vector<std::string> args;
  std::size_t length = 0;
  for (int i = 0; i < argc; i++) {

    // Convert the Unicode string to the specified character encoding
    // The function returns 0 if it does not succeed.
    length =
        static_cast<std::size_t>(WideCharToMultiByte(CP_UTF8, 0, argv[i], -1, buffer, bufferSize, nullptr, nullptr));

    // Store the converted string in the vector
    // note: "length - 1" to remove the null terminator
    std::string myarg(buffer, length - 1);
    std::cout << "arg: " << myarg << "\n";
    args.push_back(myarg);
  }

  // Free the memory allocated by CommandLineToArgvW
  LocalFree(argv);
  return args;
}
} // namespace aww::os

namespace aww::os::actions {
/**
 * @brief Launches file with OS handler. (aww tag #20250511d3)
 */
aww::Result launch_file(const std::string& path) {
  // Check if the path is empty
  if (path.empty()) {
    return aww::Result::fail("Argument path is empty");
  }

  // Prepare the command to execute
  std::string command = "cmd /c start \"\" \"" + path + "\"";

  // Set up the STARTUPINFO and PROCESS_INFORMATION structures
  STARTUPINFOA si = {0};
  si.cb = sizeof(STARTUPINFOA);
  PROCESS_INFORMATION pi = {0};

  // Create the process with the DETACHED_PROCESS flag
  BOOL success = CreateProcessA(nullptr,                             // Application name
                                command.data(),                      // Command line
                                nullptr,                             // Process security attributes
                                nullptr,                             // Thread security attributes
                                FALSE,                               // Inherit handles
                                CREATE_NO_WINDOW | DETACHED_PROCESS, // Creation flags
                                nullptr,                             // Environment
                                nullptr,                             // Current directory
                                &si,                                 // Startup info
                                &pi                                  // Process info
  );

  if (success) {
    // Close process and thread handles to avoid resource leaks
    CloseHandle(pi.hProcess);
    CloseHandle(pi.hThread);
    return aww::Result::ok();
  }

  // Handle errors
  unsigned long errorCode = GetLastError();
  if (errorCode == ERROR_FILE_NOT_FOUND) {
    return aww::Result::fail("CreateProcessA failed: The specified file was not found.");
  } else if (errorCode == ERROR_PATH_NOT_FOUND) {
    return aww::Result::fail("CreateProcessA failed: The specified path was not found.");
  } else {
    return aww::Result::fail("CreateProcessA failed: Unknown error.");
  }
}

/* WinToast */
class WinToastHandlerExample : public WinToastLib::IWinToastHandler {
public:
  WinToastHandlerExample() = default;
  // Public interfaces
  void toastActivated() const override {
  }

  void toastActivated(int) const override {
  }

  void toastDismissed(WinToastDismissalReason) const override {
  }

  void toastFailed() const override {
  }
};

/**
 * @brief Shows system notification. (aww tag #20250511d4)
 */
aww::Result show_notification(const std::string& title, const std::string& message) {
  // check title is null
  if (title.empty()) {
    return aww::Result::fail("Argument title is empty");
  }
  // check message is null
  if (message.empty()) {
    return aww::Result::fail("Argument message is empty");
  }

  auto wintoastDefaultHandler = new WinToastHandlerExample();
  WinToastLib::WinToast toast;

  toast.setAppName(L"aww");
  toast.setAppUserModelId(L"aww");

  if (!toast.initialize()) {
    return aww::Result::fail("Error, your system in not compatible!");
  }

  WinToastLib::WinToastTemplate templ = WinToastLib::WinToastTemplate(WinToastLib::WinToastTemplate::Text02);

  std::wstring wtitle(title.begin(), title.end());
  templ.setTextField(wtitle, WinToastLib::WinToastTemplate::FirstLine);

  std::wstring wmessage(message.begin(), message.end());
  templ.setTextField(wmessage, WinToastLib::WinToastTemplate::SecondLine);

  const int64_t expirationMs = 5000;
  templ.setExpiration(expirationMs);

  long long int toastId = toast.showToast(templ, wintoastDefaultHandler);
  if (toastId < 0) {
    return aww::Result::fail("Error, cannot show the toast notification");
  }
  return aww::Result::ok();
}
} // namespace aww::os::actions

namespace aww::fs {
/**
 * @brief Gets current executable path. (aww tag #20250511d5)
 */
std::filesystem::path get_current_executable_path(void) {
  char buffer[MAX_PATH];
  DWORD nchar;
  nchar = GetModuleFileNameA(nullptr, buffer, MAX_PATH);
  if (nchar == 0) {
    return std::filesystem::path("");
  }
  return std::filesystem::path(buffer);
}
} // namespace aww::fs

namespace aww::util {
/**
 * @brief Generates GUID string. (aww tag #20250511d6)
 */
aww::Result get_guid(std::string& out) {
  GUID guid;
  HRESULT hres = CoCreateGuid(&guid);
  if (FAILED(hres)) {
    return aww::Result::fail("Windows: CoCreateGuid failed. Unable to generate GUID.");
  }
  const int bufferSize = 40;
  char buffer[bufferSize];

  wchar_t guidStr[bufferSize];
  int sfgCount = StringFromGUID2(guid, guidStr, bufferSize);
  if (sfgCount == 0) {
    return aww::Result::fail("Windows: StringFromGUID2 failed. Unable to generate GUID.");
  }

  size_t length =
      static_cast<std::size_t>(WideCharToMultiByte(CP_UTF8, 0, guidStr, -1, buffer, bufferSize, nullptr, nullptr));
  if (length == 0) {
    return aww::Result::fail("Windows: WideCharToMultiByte failed. Unable to generate GUID.");
  }

  std::string guidStrResult(buffer, length - 1);
  // make lowercase
  std::transform(guidStrResult.begin(), guidStrResult.end(), guidStrResult.begin(),
                 [](unsigned char c) { return static_cast<unsigned char>(std::tolower(c)); });

  // if the first character is a '{' remove it
  if (guidStrResult[0] == '{') {
    guidStrResult.erase(0, 1);
  }
  // if the last character is a '}' remove it
  if (guidStrResult[guidStrResult.length() - 1] == '}') {
    guidStrResult.erase(guidStrResult.length() - 1, 1);
  }

  out = guidStrResult;
  return aww::Result::ok();
}
} // namespace aww::util

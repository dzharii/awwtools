#include <array>
#include <filesystem>
#include <iostream>
#include <map>
#include <string>
#include <vector>

#include "aww-common.hpp"
#include "internal/aww-run.hpp"

#include <lua.hpp> // Include Lua header
#include <spdlog/spdlog.h>

namespace fs = std::filesystem;

namespace aww::internal::aww_run {

int aww_run_main(const std::vector<std::string>& cmdArgs) {
  auto mutableCmdArgs = cmdArgs;

  bool noLogging = aww::erase_flag_from_args(mutableCmdArgs, aww::constants::CMD_FLAG_NO_LOGGING);
  bool noNotifications =
      aww::erase_flag_from_args(mutableCmdArgs, aww::constants::CMD_FLAG_NO_NOTIFICATIONS);

  // Configure spdlog based on the flag
  if (noLogging) {
    spdlog::set_level(spdlog::level::off);
  } else {
    spdlog::set_level(spdlog::level::info);
  }

  if (mutableCmdArgs.size() == 0) {
    spdlog::warn("No arguments provided");
    return 1;
  }

  std::string awwCommand = mutableCmdArgs[0];
  fs::path maybeScriptPath;

  aww::Result scriptFound = find_script(awwCommand, maybeScriptPath);
  if (scriptFound.is_ok()) {
    spdlog::warn("Found script: {}", maybeScriptPath.string());
    std::string scriptExtension = maybeScriptPath.extension().string();

    // Check if the script is a Lua file
    if (scriptExtension == ".lua" || scriptExtension == ".LUA") {
      // Initialize Lua
      lua_State* L = luaL_newstate();
      luaL_openlibs(L);

      // Measure time
      auto start = std::chrono::high_resolution_clock::now();

      // Execute Lua script file
      if (luaL_dofile(L, maybeScriptPath.string().c_str()) != LUA_OK) {
        std::cerr << "Error executing Lua script: " << lua_tostring(L, -1) << std::endl;
        lua_pop(L, 1);
        lua_close(L);
        return 1;
      }

      lua_close(L);

      // Calculate and log time taken
      auto end = std::chrono::high_resolution_clock::now();
      auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(end - start);
      spdlog::warn("Lua script execution took {}ms", duration.count());

      if (!noNotifications) {
        aww::os::actions::show_notification("aww run", "Lua script finished successfully");
      }
      return 0;
    }

    // Existing functionality for other scripts
    bool isPowerShell = scriptExtension == ".ps1" || scriptExtension == ".PS1";
    bool isBash = scriptExtension == ".sh" || scriptExtension == ".SH";
    const aww::os::Platform platform = aww::os::OSPlatform;

    switch (platform) {
    case aww::os::Platform::Windows:
      if (isPowerShell) {
        awwCommand = "powershell.exe -executionpolicy unrestricted -File \"" +
                     maybeScriptPath.string() + "\"";
      } else {
        awwCommand = "\"" + maybeScriptPath.string() + "\"";
      }
      break;
    case aww::os::Platform::Linux:
      if (isPowerShell) {
        awwCommand = "pwsh -File \"" + maybeScriptPath.string() + "\"";
      } else if (isBash) {
        awwCommand = "bash \"" + maybeScriptPath.string() + "\"";
      } else {
        awwCommand = "\"" + maybeScriptPath.string() + "\"";
      }
      break;
    case aww::os::Platform::Unknown:
      spdlog::warn("Unknown platform");
      return 1;
    }
  } else {
    spdlog::warn("Aww command was not found: {}", awwCommand);
  }

  std::vector<std::string> cmdArgsCopy = mutableCmdArgs;

  // Replace aww command with the full path to the script
  if (scriptFound.is_ok()) {
    cmdArgsCopy[0] = awwCommand;
  }

  std::string cmd = aww::string::join(cmdArgsCopy, " ");
  spdlog::warn("Running command: {}", cmd);

  aww::os::Proccess proc;
  int exitCode = 0;
  proc.onStdOut([](std::string line) { std::cout << line; });
  proc.onStdErr([](std::string line) { std::cerr << line; });

  proc.onExit([&](int code) {
    spdlog::warn("Exit code: {}", code);
    exitCode = code;
  });

  // Measure time
  auto start = std::chrono::high_resolution_clock::now();
  proc.run(cmd);
  auto end = std::chrono::high_resolution_clock::now();
  auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(end - start);
  spdlog::warn("Command took {}ms", duration.count());

  if (exitCode != 0) {
    if (!noNotifications) {
      aww::os::actions::show_notification("aww run", "Failed to run command");
    }
    spdlog::error("Failed to run command: '{}'; ErrorCode: {}", cmd, exitCode);
  } else {
    if (!noNotifications) {
      aww::os::actions::show_notification("aww run", "The command finished successfully");
    }
  }

  return exitCode;
}

aww::Result find_script_windows(const std::string& scriptName, fs::path& outScriptPath) {
  // check if scriptName is not empty
  if (scriptName.empty()) {
    return aww::Result::fail("Script name is empty");
  }

  const fs::path currentDir = fs::absolute(fs::current_path());
  const fs::path awwScriptsDir = currentDir / "aww-scripts";
  const fs::path awwDotScriptsDir = aww::os::env::get_aww_dot_dir() / "aww-scripts";
  const fs::path awwDir = currentDir / "aww";

  const fs::path batCurrentDirPath = currentDir / (scriptName + ".bat");
  const fs::path cmdCurrentDirPath = currentDir / (scriptName + ".cmd");
  const fs::path ps1CurrentDirPath = currentDir / (scriptName + ".ps1");
  const fs::path luaCurrentDirPath = currentDir / (scriptName + ".lua");

  const fs::path batAwwScriptsPath = awwScriptsDir / (scriptName + ".bat");
  const fs::path cmdAwwScriptsPath = awwScriptsDir / (scriptName + ".cmd");
  const fs::path ps1AwwScriptsPath = awwScriptsDir / (scriptName + ".ps1");
  const fs::path luaAwwScriptsPath = awwScriptsDir / (scriptName + ".lua");

  const fs::path batAwwDotScriptsPath = awwDotScriptsDir / (scriptName + ".bat");
  const fs::path cmdAwwDotScriptsPath = awwDotScriptsDir / (scriptName + ".cmd");
  const fs::path ps1AwwDotScriptsPath = awwDotScriptsDir / (scriptName + ".ps1");
  const fs::path luaAwwDotScriptsPath = awwDotScriptsDir / (scriptName + ".lua");

  const fs::path batAwwPath = awwDir / (scriptName + ".bat");
  const fs::path cmdAwwPath = awwDir / (scriptName + ".cmd");
  const fs::path ps1AwwPath = awwDir / (scriptName + ".ps1");
  const fs::path luaAwwPath = awwDir / (scriptName + ".lua");

  const size_t totalDirItems = 4 * 4;

  std::array<fs::path, totalDirItems> lookupPath = {
      batCurrentDirPath,    cmdCurrentDirPath,    ps1CurrentDirPath,    luaCurrentDirPath,
      batAwwScriptsPath,    cmdAwwScriptsPath,    ps1AwwScriptsPath,    luaAwwScriptsPath,
      batAwwDotScriptsPath, cmdAwwDotScriptsPath, ps1AwwDotScriptsPath, luaAwwDotScriptsPath,
      batAwwPath,           cmdAwwPath,           ps1AwwPath,           luaAwwPath,
  };

  for (const fs::path& path : lookupPath) {
    if (fs::exists(path)) {
      outScriptPath = path;
      return aww::Result::ok();
    }
  }
  return aww::Result::fail("Script not found");
}

aww::Result find_script_linux(const std::string& scriptName, fs::path& outScriptPath) {
  if (scriptName.empty()) {
    return aww::Result::fail("Script name is empty");
  }

  const fs::path currentDir = fs::absolute(fs::current_path());
  const fs::path awwScriptsDir = currentDir / "aww-scripts";
  const fs::path awwDotScriptsDir = aww::os::env::get_aww_dot_dir() / "aww-scripts";
  const fs::path awwDir = currentDir / "aww";

  const fs::path shCurrentDirPath = currentDir / (scriptName + ".sh");
  const fs::path shAwwScriptsPath = awwScriptsDir / (scriptName + ".sh");
  const fs::path shAwwDotScriptsPath = awwDotScriptsDir / (scriptName + ".sh");
  const fs::path shAwwPath = awwDir / (scriptName + ".sh");

  const fs::path ps1CurrentDirPath = currentDir / (scriptName + ".ps1");
  const fs::path ps1AwwScriptsPath = awwScriptsDir / (scriptName + ".ps1");
  const fs::path ps1AwwDotScriptsPath = awwDotScriptsDir / (scriptName + ".ps1");
  const fs::path ps1AwwPath = awwDir / (scriptName + ".ps1");

  const fs::path luaCurrentDirPath = currentDir / (scriptName + ".lua");
  const fs::path luaAwwScriptsPath = awwScriptsDir / (scriptName + ".lua");
  const fs::path luaAwwDotScriptsPath = awwDotScriptsDir / (scriptName + ".lua");
  const fs::path luaAwwPath = awwDir / (scriptName + ".lua");

  const fs::path emptyCurrentDirPath = currentDir / scriptName;
  const fs::path emptyAwwScriptsPath = awwScriptsDir / scriptName;
  const fs::path emptyAwwDotScriptsPath = awwDotScriptsDir / scriptName;
  const fs::path emptyPath = awwDir / scriptName;

  const std::array<fs::path, 12> shLookupPath = {
      shCurrentDirPath,  shAwwScriptsPath,  shAwwDotScriptsPath,  shAwwPath,
      ps1CurrentDirPath, ps1AwwScriptsPath, ps1AwwDotScriptsPath, ps1AwwPath,
      luaCurrentDirPath, luaAwwScriptsPath, luaAwwDotScriptsPath, luaAwwPath,
  };

  const std::array<fs::path, 4> emptyLookupPath = {
      emptyCurrentDirPath,
      emptyAwwScriptsPath,
      emptyAwwDotScriptsPath,
      emptyPath,
  };

  for (const fs::path& path : shLookupPath) {
    if (fs::exists(path)) {
      outScriptPath = path;
      return aww::Result::ok();
    }
  }

  for (const fs::path& path : emptyLookupPath) {
    if (fs::exists(path) && aww::os::can_execute(path)) {
      outScriptPath = path;
      return aww::Result::ok();
    }
  }

  return aww::Result::fail("Script not found");
}

aww::Result find_script(const std::string& scriptName, fs::path& outScriptPath) {
  if (scriptName.empty()) {
    return aww::Result::fail("Script name is empty");
  }

  const aww::os::Platform platform = aww::os::OSPlatform;

  switch (platform) {
  case aww::os::Platform::Windows:
    return find_script_windows(scriptName, outScriptPath);
  case aww::os::Platform::Linux:
    return find_script_linux(scriptName, outScriptPath);
  default:
    return aww::Result::fail("Unknown platform");
  }
}

} // namespace aww::internal::aww_run

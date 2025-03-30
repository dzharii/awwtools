#include <array>
#include <filesystem>
#include <iostream>
#include <map>
#include <string>
#include <vector>

#include "aww-common.hpp"
#include "aww-spdlog-configuration.hpp"
#include "internal/aww-run.hpp"

#include <lua.hpp> // Include Lua header
#include <spdlog/spdlog.h>

namespace fs = std::filesystem;

namespace aww::internal::aww_run {

int aww_run_main(const std::vector<std::string>& cmdArgs) {
  init_default_spdlog_configuration("aww-run");

  auto mutableCmdArgs = cmdArgs;

  bool noLogging = aww::erase_flag_from_args(mutableCmdArgs, aww::constants::CMD_FLAG_NO_LOGGING);
  bool noNotifications = aww::erase_flag_from_args(mutableCmdArgs, aww::constants::CMD_FLAG_NO_NOTIFICATIONS);

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
  fs::path scriptPath;

  aww::Result scriptFound = find_script(awwCommand, scriptPath);
  if (scriptFound.is_ok()) {
    spdlog::warn("Found script: {}", scriptPath.string());
    std::string scriptExtension = scriptPath.extension().string();

    // Check if the script is a Lua file
    if (scriptExtension == ".lua" || scriptExtension == ".LUA") {
      // Initialize Lua
      lua_State* L = luaL_newstate();
      luaL_openlibs(L);

      // Populate Lua's 'arg' table with command-line arguments
      lua_newtable(L); // Create a new table

      // add the script path as the first argument
      constexpr int LUA_TABLE_INDEX = -3; // Index of the table in the Lua stack
      lua_Number luaCmdIndex = 1;
      lua_pushnumber(L, luaCmdIndex);
      lua_pushstring(L, scriptPath.string().c_str());
      lua_settable(L, LUA_TABLE_INDEX);

      // starts from 1 because the first argument is the script path which is already added
      for (size_t i = 1; i < mutableCmdArgs.size(); ++i) {
        luaCmdIndex += 1.0;
        lua_pushnumber(L, luaCmdIndex);               // Push the index (Lua uses 1-based indexing)
        lua_pushstring(L, mutableCmdArgs[i].c_str()); // Push the argument string
        lua_settable(L, LUA_TABLE_INDEX);             // Set table[arg_index] = arg_value
      }
      lua_setglobal(L, "arg"); // Set the table as the global 'arg'

      // Measure time
      auto start = std::chrono::high_resolution_clock::now();

      // Execute Lua script file
      if (luaL_dofile(L, scriptPath.string().c_str()) != LUA_OK) {
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
        awwCommand = "powershell.exe -executionpolicy unrestricted -File \"" + scriptPath.string() + "\"";
      } else {
        awwCommand = "\"" + scriptPath.string() + "\"";
      }
      break;
    case aww::os::Platform::Linux:
      if (isPowerShell) {
        awwCommand = "pwsh -File \"" + scriptPath.string() + "\"";
      } else if (isBash) {
        awwCommand = "bash \"" + scriptPath.string() + "\"";
      } else {
        awwCommand = "\"" + scriptPath.string() + "\"";
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

struct script_search_locations {
  std::vector<fs::path> values;

  explicit script_search_locations(std::initializer_list<fs::path> paths) {
    for (const auto& path : paths) {
      values.emplace_back(path);
    }
  }
};

struct script_extensions {
  std::vector<std::string> values;

  // Constructor accepting initializer list of strings
  explicit script_extensions(std::initializer_list<std::string> extensions) {
    for (const auto& extension : extensions) {
      values.emplace_back(extension);
    }
  }
};

std::vector<fs::path> get_script_search_locations(std::string scriptName,
                                                  const script_search_locations& searchLocations,
                                                  const script_extensions& extensions) {
  std::vector<fs::path> results;
  if (scriptName.empty()) {
    return results;
  }

  for (const auto& location : searchLocations.values) {
    for (const auto& extension : extensions.values) {
      fs::path scriptPath = fs::absolute(location / (scriptName + extension));
      if (fs::exists(scriptPath)) {
        results.push_back(scriptPath);
      }
    }
  }
  return results;
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

  const script_search_locations searchLocations{currentDir, awwScriptsDir, awwDotScriptsDir, awwDir};
  const script_extensions extensions{".bat", ".cmd", ".ps1", ".lua", ""};

  std::vector<fs::path> candidates = get_script_search_locations(scriptName, searchLocations, extensions);
  if (!candidates.empty()) {
    outScriptPath = candidates[0];
    return aww::Result::ok();
  }

  return aww::Result::fail("Script not found");
}

aww::Result find_script_linux(const std::string& scriptName, fs::path& outScriptPath) {
  // check if scriptName is not empty
  if (scriptName.empty()) {
    return aww::Result::fail("Script name is empty");
  }

  const fs::path currentDir = fs::absolute(fs::current_path());
  const fs::path awwScriptsDir = currentDir / "aww-scripts";
  const fs::path awwDotScriptsDir = aww::os::env::get_aww_dot_dir() / "aww-scripts";
  const fs::path awwDir = currentDir / "aww";

  const script_search_locations searchLocations{currentDir, awwScriptsDir, awwDotScriptsDir, awwDir};
  const script_extensions extensions{".sh", ".ps1", ".lua", ""};

  std::vector<fs::path> candidates = get_script_search_locations(scriptName, searchLocations, extensions);
  if (!candidates.empty()) {
    outScriptPath = candidates[0];
    return aww::Result::ok();
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

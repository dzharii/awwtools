#include "aww-common.hpp"
#include "aww-os/aww-os.hpp"
#include <algorithm>
#include <cctype>
#include <fstream>

namespace aww {
/**
 * @brief Removes a flag from args. (aww tag #20250511a1)
 */
bool erase_flag_from_args(std::vector<std::string>& args, const std::string& flag) {
  return aww::erase_all_matched_elements(args, flag);
}
} // namespace aww

namespace aww::date {
/**
 * @brief Gets date as YYYY-MM-DD. (aww tag #20250511a3)
 */
std::string get_date_YYYYMMDD(void) {
  std::string date;
  char buff[32]{};
  time_t t = time(nullptr);

  if (std::strftime(buff, sizeof(buff), "%F", std::localtime(&t))) {
    date = buff;
  }
  return date;
}
} // namespace aww::date

namespace aww::string {
/**
 * @brief Joins vector with delimiter. (aww tag #20250511ab)
 */
std::string join(const std::vector<std::string>& v, const std::string& delimiter) {
  // Glen Knowles - https://stackoverflow.com/a/40497964
  std::string out;
  if (auto i = v.begin(), e = v.end(); i != e) {
    out += *i++;
    for (; i != e; ++i)
      out.append(delimiter).append(*i);
  }
  return out;
}

/**
 * @brief Pads string left. (aww tag #20250511ac)
 */
std::string left_padding(const std::string& inp, const char& padChar, const size_t& padLength) {
  const size_t inpLength = inp.length();
  if (inpLength >= padLength) {
    return inp;
  }
  const size_t paddingLength = padLength - inpLength;
  std::string padding(paddingLength, padChar);
  return padding + inp;
}

/**
 * @brief Converts to uppercase. (aww tag #20250511ad)
 */
std::string to_upper(const std::string& inp) {
  std::string out(inp);
  for (char& c : out) {
    c = static_cast<char>(std::toupper(c));
  }
  return out;
}

/**
 * @brief Converts to lowercase. (aww tag #20250511ae)
 */
std::string to_lower(const std::string& inp) {
  std::string out(inp);
  for (char& c : out) {
    c = static_cast<char>(std::tolower(c));
  }
  return out;
}

/**
 * @brief Checks if string ends with suffix. (aww tag #20250511af)
 */
bool ends_with(const std::string& str, const std::string& suffix) {
  if (suffix.size() > str.size()) {
    return false;
  }
  return std::equal(suffix.rbegin(), suffix.rend(), str.rbegin());
}

/**
 * @brief Makes valid identifier. (aww tag #20250511b0)
 */
std::string to_valid_identifier(const std::string& input) {
  std::string output = input;
  std::replace_if(output.begin(), output.end(), [](char c) { return !std::isalnum(c); }, '_');
  return output;
}

/**
 * @brief Trims whitespace. (aww tag #20250511b1)
 */
std::string trim(std::string str) {
  // Trim leading whitespace
  str.erase(str.begin(), std::find_if(str.begin(), str.end(), [](int ch) { return !std::isspace(ch); }));

  // Trim trailing whitespace
  str.erase(std::find_if(str.rbegin(), str.rend(), [](int ch) { return !std::isspace(ch); }).base(), str.end());

  return str;
}
} // namespace aww::string

namespace aww::os {
/**
 * @brief Proccess constructor. (aww tag #20250511a5)
 */
Proccess::Proccess() {
  this->onStdOutCallback = defaultStdOutCallback;
  this->onStdErrCallback = defaultStdErrCallback;
  this->onExitCallback = defaultExitCallback;
}

/**
 * @brief Set stdout callback. (aww tag #20250511a5a)
 */
Proccess& Proccess::onStdOut(std::function<void(const std::string)> callback) {
  onStdOutCallback = callback;
  return *this;
}

/**
 * @brief Set stderr callback. (aww tag #20250511a5b)
 */
Proccess& Proccess::onStdErr(std::function<void(const std::string)> callback) {
  onStdErrCallback = callback;
  return *this;
}

/**
 * @brief Set exit callback. (aww tag #20250511a5c)
 */
Proccess& Proccess::onExit(std::function<void(int)> callback) {
  onExitCallback = callback;
  return *this;
}

/**
 * @brief Runs command. (aww tag #20250511a5d)
 */
int Proccess::run(const std::string& cmd) {
  FILE* fp = popen(cmd.c_str(), "r");
  if (fp == nullptr) {
    throw std::runtime_error("Could not open pipe");
  }
  const size_t bufferSize = 1024;
  char buffer[bufferSize];
  while (fgets(buffer, bufferSize, fp) != nullptr) {
    onStdOutCallback(buffer);
  }
  int exitCode = pclose(fp);
  onExitCallback(exitCode);
  return exitCode;
}

/**
 * @brief Default stdout callback. (aww tag #20250511a5e)
 */
void Proccess::defaultStdOutCallback(std::string) {
}

/**
 * @brief Default stderr callback. (aww tag #20250511a5f)
 */
void Proccess::defaultStdErrCallback(std::string) {
}

/**
 * @brief Default exit callback. (aww tag #20250511a5g)
 */
void Proccess::defaultExitCallback(int) {
}
} // namespace aww::os

namespace aww::os::env {
/**
 * @brief Gets .awwtools folder. (aww tag #20250511a9)
 */
std::optional<std::filesystem::path> get_aww_dot_folder(void) {
  std::optional<std::filesystem::path> user_home_folder = aww::get_user_home_folder();

  if (!user_home_folder.has_value()) {
    return std::nullopt;
  }
  return std::filesystem::absolute(user_home_folder.value() / aww::constants::AWW_TOOLS_DOT_FOLDER_NAME);
}

/**
 * @brief Gets aww-scripts folder. (aww tag #20250511aa)
 */
std::optional<std::filesystem::path> get_aww_dot_folder_aww_scripts_folder(void) {
  std::optional<std::filesystem::path> user_home_aww_tools_folder = get_aww_dot_folder();

  if (!user_home_aww_tools_folder.has_value()) {
    return std::nullopt;
  }
  return std::filesystem::absolute(user_home_aww_tools_folder.value() / aww::constants::AWW_SCRIPTS_FOLDER_NAME);
}

} // namespace aww::os::env

namespace aww::fs {
/**
 * @brief Gets current dir absolute path. (aww tag #20250511b3)
 */
aww::Result get_current_directory_absolute_path(std::filesystem::path& result) {
  try {
    result = std::filesystem::current_path();
    result = std::filesystem::absolute(result);
    return aww::Result::ok();
  } catch (const std::exception& e) {
    std::string errorMessage = "Error getting current directory: " + std::string(e.what());
    return aww::Result::fail(errorMessage);
  } catch (...) {
    std::string errorMessage = "Unknown error occurred while getting current directory.";
    return aww::Result::fail(errorMessage);
  }
}

/**
 * @brief Checks if path is directory. (aww tag #20250511b4)
 */
aww::Result is_directory(const std::filesystem::path& path, bool& outIsDirectory) {
  try {
    outIsDirectory = std::filesystem::is_directory(path);
    return aww::Result::ok();
  } catch (const std::exception& e) {
    std::string errorMessage = "Error checking if directory: " + std::string(e.what());
    return aww::Result::fail(errorMessage);
  } catch (...) {
    std::string errorMessage = "Unknown error occurred while checking if directory.";
    return aww::Result::fail(errorMessage);
  }
}

/**
 * @brief Creates empty file. (aww tag #20250511b6)
 */
aww::Result create_empty_file(const std::filesystem::path& path) {
  try {
    std::ofstream file(path);
    file.close();
    return aww::Result::ok();
  } catch (const std::exception& e) {
    std::string errorMessage = "Error creating file '" + path.filename().string() + "': " + e.what();
    return aww::Result::fail(errorMessage);
  } catch (...) {
    std::string errorMessage = "Unknown error occurred while creating file '" + path.filename().string() + "'.";
    return aww::Result::fail(errorMessage);
  }
}

/**
 * @brief Reads lines from file. (aww tag #20250511b7)
 */
aww::Result read_lines(const std::filesystem::path& filePath, std::vector<std::string>& outFileLines) {
  try {
    std::ifstream file(filePath);
    if (!file.is_open()) {
      return aww::Result::fail("Failed to open file for reading.");
    }

    std::string line;
    while (std::getline(file, line)) {
      outFileLines.push_back(line);
    }

    file.close();
    return aww::Result::ok();
  } catch (const std::exception& e) {
    std::string errorMessage = "Error reading file: " + std::string(e.what());
    return aww::Result::fail(errorMessage);
  } catch (...) {
    std::string errorMessage = "Unknown error occurred while reading file.";
    return aww::Result::fail(errorMessage);
  }
}

/**
 * @brief Writes lines to file. (aww tag #20250511b9)
 */
aww::Result write_lines(const std::filesystem::path& filePath, const std::vector<std::string>& lines) {
  try {
    std::ofstream file(filePath);
    if (!file.is_open()) {
      return aww::Result::fail("Failed to open file for writing.");
    }

    for (const auto& line : lines) {
      file << line << std::endl;
    }

    file.close();
    return aww::Result::ok();
  } catch (const std::exception& e) {
    std::string errorMessage = "Error writing file: " + std::string(e.what());
    return aww::Result::fail(errorMessage);
  } catch (...) {
    std::string errorMessage = "Unknown error occurred while writing file.";
    return aww::Result::fail(errorMessage);
  }
}

/**
 * @brief Reads ASCII text file. (aww tag #20250511b8)
 */
std::string read_ascii_text_file(const std::filesystem::path& path) {
  std::ifstream file(path);
  std::string content((std::istreambuf_iterator<char>(file)), std::istreambuf_iterator<char>());
  return content;
}

/**
 * @brief Normalizes file path. (aww tag #20250511ba)
 */
std::string normalize_file_path(const std::string& inputPath) {
  std::filesystem::path path(inputPath);
  std::filesystem::path canonicalPath = std::filesystem::weakly_canonical(path);
  std::string normalizedPath = canonicalPath.make_preferred().string();
  return normalizedPath;
}
} // namespace aww::fs

#include "aww-app-manifest.hpp"
#include "nlohmann/json.hpp"

/// @brief Constructor for aww::app::manifest::description
aww::app::manifest::title::title(const std::string& titleValue)
{
  if (titleValue.length() > 255)
  {
    throw std::invalid_argument("titleValue must not be longer than 255 characters");
  }
  value = std::string(titleValue);
}

/// @brief Get the title
std::string aww::app::manifest::title::get(void)
{
  return std::string(value);
}


/// @brief Constructor for aww::app::manifest::description
aww::app::manifest::description::description(const std::string& descriptionValue)
{
  if (descriptionValue.length() > 2048)
  {
    throw std::invalid_argument("descriptionValue must not be longer than 2048 characters");
  }

  value = std::string(descriptionValue);
}

/// @brief Get the description
std::string aww::app::manifest::description::get(void)
{
  return std::string(value);
}

/// @brief Constructor for aww::app::manifest::version
aww::app::manifest::version::version(const std::string& versionValue)
{
  if (versionValue.length() > 255)
  {
    throw std::invalid_argument("versionValue must not be longer than 255 characters");
  }

  value = std::string(versionValue);
}

/// @brief Get the version
std::string aww::app::manifest::version::get(void)
{
  return std::string(value);
}

/// @brief Constructor for aww::app::manifest::mainJavascriptFile
/// @param mainJavascriptFileValue
aww::app::manifest::mainJavascriptFile::mainJavascriptFile(
  const std::filesystem::path& mainJavascriptFileValue,
  const std::filesystem::path& appDirPath)
{
  // mainJavascriptFileValue is not empty
  if (mainJavascriptFileValue.empty())
  {
    throw std::invalid_argument("mainJavascriptFile must not be empty");
  }
  std::filesystem::path filePath = std::filesystem::absolute(
    appDirPath / mainJavascriptFileValue);

  // filePath is a child of appDirPath
  if (filePath.string().find(appDirPath.string()) == std::string::npos)
  {
    throw std::invalid_argument("mainJavascriptFile must be a child of appDirPath");
  }
  value = filePath;
}

/// @brief Get the mainJavascriptFile
std::filesystem::path aww::app::manifest::mainJavascriptFile::get(void)
{
  return std::filesystem::path(value);
}

/// @brief Constructor for aww::app::manifest::appmanifest
aww::app::manifest::appmanifest::appmanifest(
      const aww::app::manifest::title& appTitle,
      const aww::app::manifest::description& appDescription,
      const aww::app::manifest::version& appVersion,
      const aww::app::manifest::mainJavascriptFile& appMainJavascriptFile)
:
  title(appTitle),
  description(appDescription),
  version(appVersion),
  mainJavascriptFile(appMainJavascriptFile)
{
}

/// @brief Get the appmanifest from a json object
aww::app::manifest::appmanifest aww::app::manifest::appmanifest::fromJson(
  const nlohmann::json& json,
  const std::filesystem::path& appDirPath)
{
  std::string title = json.value("title", "");
  std::string description = json.value("description", "");
  std::string version = json.value("version", "0.0.0");
  std::string mainJavascriptFile = json.value("mainJavascriptFile", "");

  aww::app::manifest::appmanifest appmanifest(
    aww::app::manifest::title(title),
    aww::app::manifest::description(description),
    aww::app::manifest::version(version),
    aww::app::manifest::mainJavascriptFile(mainJavascriptFile, appDirPath));

  return appmanifest;
}


#include "aww-app-manifest.hpp"
#include "nlohmann/json.hpp"

/// @brief Constructor for aww::app::manifest::description
/// @param titleValue
aww::app::manifest::title::title(const std::string& titleValue)
{
  if (titleValue.length() > 255)
  {
    throw std::invalid_argument("titleValue must not be longer than 255 characters");
  }
  value = std::string(titleValue);
}

/// @brief Get the title
/// @param  void
std::string aww::app::manifest::title::get(void)
{
  return std::string(value);
}


/// @brief Constructor for aww::app::manifest::description
/// @param descriptionValue
aww::app::manifest::description::description(const std::string& descriptionValue)
{
  if (descriptionValue.length() > 2048)
  {
    throw std::invalid_argument("descriptionValue must not be longer than 2048 characters");
  }

  value = std::string(descriptionValue);
}

/// @brief Get the description
/// @param  void
std::string aww::app::manifest::description::get(void)
{
  return std::string(value);
}

/// @brief Constructor for aww::app::manifest::version
/// @param versionValue
aww::app::manifest::version::version(const std::string& versionValue)
{
  if (versionValue.length() > 255)
  {
    throw std::invalid_argument("versionValue must not be longer than 255 characters");
  }

  value = std::string(versionValue);
}

/// @brief Get the version
/// @param  void
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
}

/// @brief Get the mainJavascriptFile
/// @param  void
std::filesystem::path aww::app::manifest::mainJavascriptFile::get(void)
{
  return std::filesystem::path(value);
}

/// @brief Constructor for aww::app::manifest::appmanifest
/// @param json
/// @param appDirPath
aww::app::manifest::appmanifest::appmanifest(
      const aww::app::manifest::title&,
      const aww::app::manifest::description&,
      const aww::app::manifest::version&,
      const aww::app::manifest::mainJavascriptFile&)
:
  title(title),
  description(description),
  version(version),
  mainJavascriptFile(mainJavascriptFile)
{
}

/// @brief Get the appmanifest from a json object
/// @param json
/// @param appDirPath
aww::app::manifest::appmanifest aww::app::manifest::appmanifest::fromJson(
  const nlohmann::json& json,
  const std::filesystem::path& appDirPath)
{
  std::string title = json.value("title", "");
  std::string description = json.value("description", "");
  std::string version = json.value("version", "");
  std::string mainJavascriptFile = json.value("mainJavascriptFile", "");

  aww::app::manifest::appmanifest appmanifest(
    aww::app::manifest::title(title),
    aww::app::manifest::description(description),
    aww::app::manifest::version(version),
    aww::app::manifest::mainJavascriptFile(mainJavascriptFile, appDirPath));

  return appmanifest;
}


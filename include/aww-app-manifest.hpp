#pragma once
#ifndef AWW_APP_MANIFEST_HPP
#define AWW_APP_MANIFEST_HPP

#include <string>
#include <stdexcept>
#include <filesystem>
#include "nlohmann/json.hpp"

namespace aww::app::manifest
{
  class title
  {
  public:
    std::string get(void);
    title(const std::string&);
    title(void) = delete;
  private:
    std::string value;
  };

  class description
  {
    public:
      std::string get(void);
      description(const std::string&);
      description(void) = delete;
    private:
      std::string value;
  };

  class version
  {
    public:
      std::string get(void);
      version(const std::string&);
      version(void) = delete;
    private:
      std::string value;
  };

  class mainJavascriptFile
  {
    public:
      std::filesystem::path get(void);
      mainJavascriptFile(
        const std::filesystem::path&,
        const std::filesystem::path& appDirPath);
      mainJavascriptFile(void) = delete;
    private:
      std::filesystem::path value;
  };

  class appmanifest
  {
  public:
    aww::app::manifest::title title;
    aww::app::manifest::description description;
    aww::app::manifest::version version;
    aww::app::manifest::mainJavascriptFile mainJavascriptFile;

    static aww::app::manifest::appmanifest fromJson(
      const nlohmann::json&,
      const std::filesystem::path&);

    appmanifest(
      const aww::app::manifest::title&,
      const aww::app::manifest::description&,
      const aww::app::manifest::version&,
      const aww::app::manifest::mainJavascriptFile&);
    appmanifest(void) = delete;

  };
} // namespace aww::app::manifest

#endif

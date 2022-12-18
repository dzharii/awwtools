// Executables must have the following defined if the library contains
// doctest definitions. For builds with this disabled, e.g. code shipped to
// users, this can be left out.
#ifdef ENABLE_DOCTEST_IN_LIBRARY
#define DOCTEST_CONFIG_IMPLEMENT
#include "doctest/doctest.h"
#endif

#include <iostream>
#include <fstream>
#include <string>

#include "aww-common.hpp"
#include "aww-app-manifest.hpp"
#include "webview.h"
#include "nlohmann/json.hpp"

namespace fs = std::filesystem;

int main()
{
  const fs::path myPath = fs::absolute(fs::current_path());
  const fs::path appmanifestJsonFile = myPath / "appmanifest.json";

  if (!fs::exists(appmanifestJsonFile))
  {
    std::cout << "appmanifest.json not found" << std::endl;
    return 1;
  }

  try
  {
    std::ifstream appmanifestFile(appmanifestJsonFile);
    nlohmann::json appmanifestJson = nlohmann::json::parse(appmanifestFile);

    // log appmanifestJson["description"]
    std::cout << "appmanifestJson[\"description\"]: " << appmanifestJson["description"] << std::endl;

    std::string myCommandlineArgs = aww::string::join(aww::os::getCommandLineArgs(), "<br />");
    std::cout << "myCommandlineArgs: " << myCommandlineArgs << std::endl;

    // TODO Create a class for this with default values
    const std::string manifestAppDescription = appmanifestJson["description"] == nullptr ? "" : appmanifestJson["description"];
    const std::string manifestAppTitle = appmanifestJson["title"] == nullptr ? "noname" : appmanifestJson["title"];
    aww::app::manifest::appmanifest appmanifest =
      aww::app::manifest::appmanifest::fromJson(appmanifestJson, myPath);

    webview::webview w(false, nullptr);
    w.set_title(manifestAppTitle);
    w.set_size(480, 320, WEBVIEW_HINT_NONE);
    w.set_html(aww::string::join(
        {"<!DOCTYPE html>",
         "<html>",
         "<head>",
         "<meta charset=\"utf-8\">",
         "<title>Basic Example</title>",
         "</head>",
         "<body>",
         "<div id=\"app\">",
         "<h1>Basic Example</h1>",
         "<p>appmanifestJson[\"description\"]: " + manifestAppDescription + "</p>",
         "<p>myCommandlineArgs: " + myCommandlineArgs + "</p>",
         "</div>",
         "</body>",
         "</html>"},
        "\n"));

    fs::path mainJavascriptFile = appmanifest.mainJavascriptFile.get();

    // log mainJavascriptFile
    std::cout << "mainJavascriptFile: " << mainJavascriptFile << std::endl;

    if (fs::exists(mainJavascriptFile))
    {
      // read file into string
      std::ifstream mainJavascriptFileContentsFile(mainJavascriptFile);
      std::string mainJavascriptFileContents((std::istreambuf_iterator<char>(mainJavascriptFileContentsFile)),
                                             std::istreambuf_iterator<char>());

      // log mainJavascriptFileContents
      std::cout << "mainJavascriptFileContents: " << mainJavascriptFileContents << std::endl;

      w.eval(mainJavascriptFileContents);
    }
    else
    {
      std::cout << "mainJavascriptFile not found" << std::endl;
      return 1;
    }

    w.run();
  }
  catch (const std::exception &e)
  {
    std::cout << "Exception: " << e.what() << std::endl;
    return 1;
  }

  return 0;
}

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
#include "webview.h"
#include "nlohmann-json.hpp"

namespace fs = std::filesystem;

#ifdef _WIN32
// TODO: it compiles, but how do I hide the console window?
int WINAPI WinMain(HINSTANCE, HINSTANCE, LPSTR, int) {
#else
int main() {
#endif
  const fs::path myPath = fs::absolute(fs::current_path());
  const fs::path appmanifestJsonFile = myPath / "appmanifest.json";

  if (!fs::exists(appmanifestJsonFile))
  {
    std::cout << "appmanifest.json not found" << std::endl;
    return 1;
  }

  std::ifstream appmanifestFile(appmanifestJsonFile);
  nlohmann::json appmanifestJson = nlohmann::json::parse(appmanifestFile);

  // log appmanifestJson["description"]
  std::cout << "appmanifestJson[\"description\"]: " << appmanifestJson["description"] << std::endl;


  std::string myCommandlineArgs = aww::string::join(aww::os::getCommandLineArgs(), "<br />");
  std::cout << "myCommandlineArgs: " << myCommandlineArgs << std::endl;

  webview::webview w(false, nullptr);
  w.set_title("Basic Example");
  w.set_size(480, 320, WEBVIEW_HINT_NONE);
  w.set_html("Thanks for using webview! <br/> " + myCommandlineArgs);
  w.run();

  return 0;
}

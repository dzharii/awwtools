#include <iostream>
#include <string>

#include "fmt/core.h"
#include "spdlog/spdlog.h"
/* #include <cpr/cpr.h> */

#include "aww-spdlog-configuration.hpp"
#include "internal/aww-api.hpp"

namespace aww::internal::aww_api {

int aww_api_main([[maybe_unused]] const std::vector<std::string>& cmdArgs, aww_api_io_dependencies_interface& deps) {

  init_default_spdlog_configuration("aww-api");
  spdlog::info("Hello spdlog");
  fmt::print("Hello, world from fmt PLEASE REMOVE THIS \b!\n");

  std::string requestUrl = "https://example.com";
  if (cmdArgs.size() > 0 && !cmdArgs[0].empty()) {
    requestUrl = cmdArgs[0];
  }
  /*

  cpr::Response r = cpr::Get(cpr::Url{requestUrl});

  fmt::print("Status code: {}\n", r.status_code);
  fmt::print("Content type: {}\n", r.header["content-type"]);
  fmt::print("Text: {}\n", r.text);
  */
  deps.show_notification("aww api", "It works", aww::call_tag("9mnoizxrhdw"));
  return 0;
}

} // namespace aww::internal::aww_api

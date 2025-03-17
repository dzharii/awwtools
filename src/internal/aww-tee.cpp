#include <iostream>
#include <string>

#include "fmt/core.h"
#include "spdlog/spdlog.h"

#include "aww-common.hpp"
#include "clip.h"

#include <fstream>
#include <functional>
#include <iostream>
#include <string>
#include <vector>

#include "aww-result/aww-result.hpp"
#include "fmt/core.h"
#include "spdlog/spdlog.h"

#include "internal/aww-tee.hpp"

namespace aww::internal::aww_tee {

int aww_tee_main([[maybe_unused]] const std::vector<std::string>& cmd_args,
                 [[maybe_unused]] aww_tee_io_dependencies_interface& deps) {
  spdlog::info("aww tee");

  return -1;
}

} // namespace aww::internal::aww_tee

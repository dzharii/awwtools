#pragma once

#include <string>
#include <vector>
#include <tuple>

#include <cstdio>
#include <ctime>
#include <stdlib.h>

namespace aww {

const int result = 0;
const int error = 0;

} // namespace aww

namespace aww::date {
  std::string getDateYYYYMMDD(void);
}

namespace aww::os::actions {
    std::tuple<bool, std::string> launchFile(const std::string&);
}

namespace aww::string {
  std::string join(const std::vector<std::string>&, const std::string&);
}

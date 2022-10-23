#pragma once
#ifndef AWW_COMMON_HPP
#define AWW_COMMON_HPP

#include <string>
#include <vector>
#include <tuple>

#include <cstdio>
#include <ctime>
#include <stdlib.h>

namespace aww
{

  const int resultPos = 0;
  const int errorPos = 1;

  typedef std::tuple<bool, std::string> result_t;

  bool failed(const result_t&);
  bool succeeded(const result_t&);
  std::string make_error(const std::string&, const result_t&);

} // namespace aww

namespace aww::date
{
  std::string getDateYYYYMMDD(void);
}

namespace aww::os::actions
{
  aww::result_t launchFile(const std::string &);
}

namespace aww::string
{
  std::string join(const std::vector<std::string> &, const std::string &);
}

#endif // AWW_COMMON_HPP

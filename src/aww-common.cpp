#include "aww-common.hpp"

namespace aww::date {
  // TODO: - 2022-10-18 [Exploring C++11, part 2 localtime and time again Kjellkod's Blog](https://kjellkod.wordpress.com/2013/01/22/exploring-c11-part-2-localtime-and-time-again/)
  std::string getDateYYYYMMDD(void) {
      std::string date;
      char buff[32]{};
      time_t t = time(nullptr);

      if (std::strftime(buff, sizeof(buff), "%F", std::localtime(&t))) {
          date = buff;
      }
      return date;
  }
}

namespace aww::string {
  std::string join(const std::vector<std::string> & v, const std::string& delimiter) {
    // Glen Knowles - https://stackoverflow.com/a/40497964
    std::string out;
    if (auto i = v.begin(), e = v.end(); i != e) {
        out += *i++;
        for (; i != e; ++i) out.append(delimiter).append(*i);
    }
    return out;
  }
}

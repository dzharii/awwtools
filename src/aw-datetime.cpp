#include "aw-datetime.hpp"


// See also:
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

#include "aw-datetime.hpp"

std::string getDateYYYYMMDD(void) {
    std::string date;
    char buff[32]{};
    time_t t = time(nullptr);

    if (std::strftime(buff, sizeof(buff), "%F", std::localtime(&t))) {
        date = buff;
    }
    return date;
}
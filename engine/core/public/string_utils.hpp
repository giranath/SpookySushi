#ifndef SPOOKYSUSHI_STRING_UTILS_HPP
#define SPOOKYSUSHI_STRING_UTILS_HPP

#include <string>

namespace sushi {

template<typename... Args>
typename std::enable_if<(sizeof...(Args) > 0), std::string>::type formatted_string(const char* format, Args... args) {
    int size = std::snprintf(nullptr, 0, format, args...);
    char* buffer = new char[size + 1];
    int res = std::sprintf(buffer, format, args...);

    if(res == -1) {
        return std::string{};
    }

    std::string formatted_string(buffer);

    delete[] buffer;

    return formatted_string;
}

template<typename... Args>
typename std::enable_if<(sizeof...(Args) == 0), std::string>::type formatted_string(const char* format) {
    return std::string{format};
}

}

#endif //SPOOKYSUSHI_STRING_UTILS_HPP

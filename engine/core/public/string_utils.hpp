#ifndef SPOOKYSUSHI_STRING_UTILS_HPP
#define SPOOKYSUSHI_STRING_UTILS_HPP

#include <string>

namespace sushi {

template<typename... Args>
typename std::enable_if<(sizeof...(Args) > 0), std::string>::type formatted_string(const char* format, Args... args) {
    char* raw_ptr = nullptr;
    int res = asprintf(&raw_ptr, format, args...);

    if(res == -1) {
        return std::string{};
    }

    std::string formatted_string(raw_ptr);

    std::free(raw_ptr);

    return formatted_string;
}

template<typename... Args>
typename std::enable_if<(sizeof...(Args) == 0), std::string>::type formatted_string(const char* format) {
    return std::string{format};
}

}

#endif //SPOOKYSUSHI_STRING_UTILS_HPP

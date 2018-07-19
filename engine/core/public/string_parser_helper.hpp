#ifndef SPOOKYSUSHI_STRINGPARSERHELPER_HPP
#define SPOOKYSUSHI_STRINGPARSERHELPER_HPP

#include <string>
#include <stdexcept>

namespace sushi {

template<typename Type>
struct StringParserHelper {
    using value_type = Type;

    static Type parse(const std::string& str) {
        return Type{};
    }
};

//
// Signed integers
//

template<>
struct StringParserHelper<int8_t> {
    using value_type = int8_t;

    static value_type parse(const std::string& str) {
        int32_t parsed_integer = std::stoi(str);
        if(parsed_integer > std::numeric_limits<value_type>::max() || parsed_integer < std::numeric_limits<value_type>::min()) {
            throw std::out_of_range{"parsed integer is too large"};
        }

        return static_cast<value_type>(parsed_integer);
    }
};

template<>
struct StringParserHelper<int16_t> {
    using value_type = int16_t;

    static value_type parse(const std::string& str) {
        int32_t parsed_integer = std::stoi(str);
        if(parsed_integer > std::numeric_limits<value_type>::max() || parsed_integer < std::numeric_limits<value_type>::min()) {
            throw std::out_of_range{"parsed integer is too large"};
        }

        return static_cast<value_type>(parsed_integer);
    }
};

template<>
struct StringParserHelper<int32_t> {
    using value_type = int32_t;

    static value_type parse(const std::string& str) {
        return std::stoi(str);
    }
};

template<>
struct StringParserHelper<int64_t> {
    using value_type = int64_t;

    static value_type parse(const std::string& str) {
        return std::stol(str);
    }
};

// Unsigned integers
template<>
struct StringParserHelper<uint8_t> {
    using value_type = uint8_t;

    static value_type parse(const std::string& str) {
        uint64_t parsed_integer = std::stoul(str);
        if(parsed_integer > std::numeric_limits<value_type>::max()) {
            throw std::out_of_range{"parsed integer is too large"};
        }

        return static_cast<value_type>(parsed_integer);
    }
};

template<>
struct StringParserHelper<uint16_t> {
    using value_type = uint16_t;

    static value_type parse(const std::string& str) {
        uint64_t parsed_integer = std::stoul(str);
        if(parsed_integer > std::numeric_limits<value_type>::max()) {
            throw std::out_of_range{"parsed integer is too large"};
        }

        return static_cast<value_type>(parsed_integer);
    }
};

template<>
struct StringParserHelper<uint32_t> {
    using value_type = uint32_t;

    static value_type parse(const std::string& str) {
        uint64_t parsed_integer = std::stoul(str);
        if(parsed_integer > std::numeric_limits<value_type>::max()) {
            throw std::out_of_range{"parsed integer is too large"};
        }

        return static_cast<value_type>(parsed_integer);
    }
};

template<>
struct StringParserHelper<uint64_t> {
    using value_type = uint64_t;

    static value_type parse(const std::string& str) {
        return std::stoul(str);
    }
};

}

#endif //SPOOKYSUSHI_STRINGPARSERHELPER_HPP

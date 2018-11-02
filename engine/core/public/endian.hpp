#ifndef SPOOKYSUSHI_ENDIAN_HPP
#define SPOOKYSUSHI_ENDIAN_HPP

#include <cstdint>
#include <type_traits>

namespace sushi {

namespace {

union U32F32 {
    uint32_t as_uint32_t;
    float as_float;
};

union U64F64 {
    uint64_t as_uint64_t;
    double as_double;
};

}

#if __BYTE_ORDER__ == __ORDER_LITTLE_ENDIAN__
constexpr bool is_big_endian = false;

constexpr uint8_t to_big_endian(uint8_t value) noexcept {
    return value;
}

constexpr uint16_t to_big_endian(uint16_t value) noexcept {
    return static_cast<uint16_t>(((value & 0x00FF) << 8)
                               | ((value & 0xFF00) >> 8));
}

constexpr uint32_t to_big_endian(uint32_t value) noexcept {
    return ((value & 0x000000FF) << 24)
         | ((value & 0xFF000000) >> 24)
         | ((value & 0x0000FF00) << 8)
         | ((value & 0x00FF0000) >> 8);
}

constexpr uint64_t to_big_endian(uint64_t value) noexcept {
    return ((value & 0x00000000000000FF) << 56)
         | ((value & 0xFF00000000000000) >> 56)
         | ((value & 0x000000000000FF00) << 40)
         | ((value & 0x00FF000000000000) >> 40)
         | ((value & 0x0000000000FF0000) << 24)
         | ((value & 0x0000FF0000000000) >> 24)
         | ((value & 0x00000000FF000000) << 8)
         | ((value & 0x000000FF00000000) >> 8);
}

float to_big_endian(float value) noexcept {
    U32F32 raw_value;
    raw_value.as_float = value;

    raw_value.as_uint32_t = to_big_endian(raw_value.as_uint32_t);

    return raw_value.as_float;
}


double to_big_endian(double value) noexcept {
    U64F64 raw_value;
    raw_value.as_double = value;

    raw_value.as_uint64_t = to_big_endian(raw_value.as_uint64_t);

    return raw_value.as_double;
}

constexpr uint8_t to_small_endian(uint8_t value) noexcept { return value; }
constexpr uint16_t to_small_endian(uint16_t value) noexcept { return value; }
constexpr uint32_t to_small_endian(uint32_t value) noexcept { return value; }
constexpr uint64_t to_small_endian(uint64_t value) noexcept { return value; }
float to_small_endian(float value) noexcept { return value; }
double to_small_endian(double value) noexcept { return value; }

#elif __BYTE_ORDER__ == __ORDER_BIG_ENDIAN__
constexpr bool is_big_endian = true;

constexpr uint8_t to_big_endian(uint8_t value) noexcept { return value; }
constexpr uint16_t to_big_endian(uint16_t value) noexcept { return value; }
constexpr uint32_t to_big_endian(uint32_t value) noexcept { return value; }
constexpr uint64_t to_big_endian(uint64_t value) noexcept { return value; }
float to_big_endian(float value) noexcept { return value; }
double to_big_endian(double value) noexcept { return value; }

constexpr uint8_t to_small_endian(uint8_t value) noexcept {
    return value;
}

constexpr uint16_t to_small_endian(uint16_t value) noexcept {
    return static_cast<uint16_t>(((value & 0x00FF) << 8)
                               | ((value & 0xFF00) >> 8));
}

constexpr uint32_t to_small_endian(uint32_t value) noexcept {
    return ((value & 0x000000FF) << 24)
         | ((value & 0xFF000000) >> 24)
         | ((value & 0x0000FF00) << 8)
         | ((value & 0x00FF0000) >> 8);
}

constexpr uint64_t to_small_endian(uint64_t value) noexcept {
    return ((value & 0x00000000000000FF) << 56)
         | ((value & 0xFF00000000000000) >> 56)
         | ((value & 0x000000000000FF00) << 40)
         | ((value & 0x00FF000000000000) >> 40)
         | ((value & 0x0000000000FF0000) << 24)
         | ((value & 0x0000FF0000000000) >> 24)
         | ((value & 0x00000000FF000000) << 8)
         | ((value & 0x000000FF00000000) >> 8);
}

float to_small_endian(float value) noexcept {
    U32F32 raw_value;
    raw_value.as_float = value;

    raw_value.as_uint32_t = to_small_endian(raw_value.as_uint32_t);

    return raw_value.as_float;
}


double to_small_endian(double value) noexcept {
    U64F64 raw_value;
    raw_value.as_double = value;

    raw_value.as_uint64_t = to_small_endian(raw_value.as_uint64_t);

    return raw_value.as_double;
}


#else
#error "Byte order not specified"
#endif


constexpr uint8_t to_network_endian(uint8_t value) noexcept {
    return to_big_endian(value);
}

constexpr uint16_t to_network_endian(uint16_t value) noexcept {
    return to_big_endian(value);
}

constexpr uint32_t to_network_endian(uint32_t value) noexcept {
    return to_big_endian(value);
}

constexpr uint64_t to_network_endian(uint64_t value) noexcept {
    return to_big_endian(value);
}

float to_network_endian(float value) noexcept {
    return to_big_endian(value);
}

double to_network_endian(double value) noexcept {
    return to_big_endian(value);
}

constexpr uint8_t from_network_endian(uint8_t value) noexcept {
#if __BYTE_ORDER__ == __ORDER_LITTLE_ENDIAN__
    return to_small_endian(value);
#else
    return value;
#endif
}

constexpr uint16_t from_network_endian(uint16_t value) noexcept {
#if __BYTE_ORDER__ == __ORDER_LITTLE_ENDIAN__
    return to_small_endian(value);
#else
    return value;
#endif
}

constexpr uint32_t from_network_endian(uint32_t value) noexcept {
#if __BYTE_ORDER__ == __ORDER_LITTLE_ENDIAN__
    return to_small_endian(value);
#else
    return value;
#endif
}

constexpr uint64_t from_network_endian(uint64_t value) noexcept {
#if __BYTE_ORDER__ == __ORDER_LITTLE_ENDIAN__
    return to_small_endian(value);
#else
    return value;
#endif
}

float from_network_endian(float value) noexcept {
#if __BYTE_ORDER__ == __ORDER_LITTLE_ENDIAN__
    return to_small_endian(value);
#else
    return value;
#endif
}


double from_network_endian(double value) noexcept {
#if __BYTE_ORDER__ == __ORDER_LITTLE_ENDIAN__
    return to_small_endian(value);
#else
    return value;
#endif
}

}

#endif //SPOOKYSUSHI_ENDIAN_HPP

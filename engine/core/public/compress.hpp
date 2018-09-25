#ifndef SPOOKYSUSHI_COMPRESS_HPP
#define SPOOKYSUSHI_COMPRESS_HPP

#include <vector>
#include <cstdint>

namespace sushi {

namespace internal {
    std::size_t compress(const uint8_t *input, std::size_t input_len, uint8_t *output, std::size_t output_len);
    std::size_t uncompress(const uint8_t *input, std::size_t input_len, uint8_t *output, std::size_t output_len);
}

template<typename InputIterator, typename OutputIterator>
OutputIterator compress(InputIterator begin, InputIterator end, OutputIterator dest) {
    static_assert(std::is_pod_v<typename std::iterator_traits<InputIterator>::value_type>, "value_type must be a primitive");
    const std::size_t DATA_SIZE = sizeof(typename std::iterator_traits<InputIterator>::value_type);

    // Agglomerates all the byte in a single buffer
    std::vector<uint8_t> bytes;
    for(; begin != end; ++begin) {
        const uint8_t* value_bytes = reinterpret_cast<const uint8_t*>(&(*begin));
        std::copy(value_bytes + 0, value_bytes + DATA_SIZE, std::back_inserter(bytes));
    }

    std::vector<uint8_t> output_bytes(bytes.size(), 0);
    const std::size_t compressed_size = internal::compress(bytes.data(), bytes.size(), output_bytes.data(), output_bytes.size());

    for(std::size_t i = 0; i < compressed_size; ++i) {
        *dest++ = output_bytes[i];
    }

    return dest;
}

template<typename InputIterator, typename OutputIterator>
OutputIterator decompress(InputIterator begin, InputIterator end, OutputIterator dest, std::size_t size_hint) {
    const std::size_t DATA_SIZE = sizeof(typename std::iterator_traits<InputIterator>::value_type);

    std::vector<uint8_t> bytes;
    for(; begin != end; ++begin) {
        const uint8_t* value_bytes = reinterpret_cast<const uint8_t*>(&(*begin));
        std::copy(value_bytes + 0, value_bytes + DATA_SIZE, std::back_inserter(bytes));
    }

    std::vector<uint8_t> uncompressed_bytes(size_hint, 0);
    const std::size_t uncompressed_size = internal::uncompress(bytes.data(), bytes.size(), uncompressed_bytes.data(), uncompressed_bytes.size());

    for(std::size_t i = 0; i < uncompressed_size; ++i) {
        *dest++ = uncompressed_bytes[i];
    }

    return dest;
}

}

#endif //SPOOKYSUSHI_COMPRESS_HPP

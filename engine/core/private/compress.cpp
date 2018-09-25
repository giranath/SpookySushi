#include "compress.hpp"

#include <zlib.h>
#include <cassert>
#include <limits>

namespace sushi {

std::size_t internal::compress(const uint8_t* input, std::size_t input_len, uint8_t* output, std::size_t output_len) {
    assert(input_len < std::numeric_limits<uInt>::max());
    assert(input != nullptr);
    assert(output_len < std::numeric_limits<uInt>::max());
    assert(output != nullptr);

    z_stream compression_stream;
    compression_stream.zalloc = Z_NULL;
    compression_stream.zfree = Z_NULL;
    compression_stream.opaque = Z_NULL;
    compression_stream.avail_in = static_cast<uInt>(input_len);
    compression_stream.next_in = reinterpret_cast<Bytef*>(const_cast<uint8_t*>(input));
    compression_stream.avail_out = static_cast<uInt>(output_len);
    compression_stream.next_out = reinterpret_cast<Byte*>(output);

    deflateInit(&compression_stream, Z_BEST_COMPRESSION);
    deflate(&compression_stream, Z_FINISH);
    deflateEnd(&compression_stream);

    return compression_stream.total_out;
}

std::size_t internal::uncompress(const uint8_t* input, std::size_t input_len, uint8_t *output, std::size_t output_len) {
    assert(input_len < std::numeric_limits<uInt>::max());
    assert(input != nullptr);
    assert(output_len < std::numeric_limits<uInt>::max());
    assert(output != nullptr);

    // Uncompress
    z_stream inflate_stream;
    inflate_stream.zalloc = Z_NULL;
    inflate_stream.zfree = Z_NULL;
    inflate_stream.opaque = Z_NULL;
    inflate_stream.avail_in = static_cast<uInt>(input_len);
    inflate_stream.next_in = static_cast<Bytef*>(const_cast<uint8_t*>(input));
    inflate_stream.avail_out = static_cast<uInt>(output_len);
    inflate_stream.next_out = static_cast<Bytef*>(output);

    inflateInit(&inflate_stream);
    inflate(&inflate_stream, Z_NO_FLUSH);
    inflateEnd(&inflate_stream);

    return inflate_stream.total_out;
}

}
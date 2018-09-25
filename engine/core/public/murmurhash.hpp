#ifndef SPOOKYSUSHI_MURMURHASH_HPP
#define SPOOKYSUSHI_MURMURHASH_HPP

#include <cstdint>

namespace sushi {

uint32_t murmur_hash3_x86_32(const void* key, std::size_t length, uint32_t seed);

}

#endif

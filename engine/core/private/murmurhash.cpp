#include "murmurhash.hpp"

namespace sushi {

static uint32_t rot32(uint32_t x, int8_t r) {
    return (x << r) | (x >> (32 - r));
}

static uint32_t fmix32(uint32_t h) {
    h ^= h >> 16;
    h *= 0x85ebca6b;
    h ^= h >> 13;
    h *= 0xc2b2ae35;
    h ^= h >> 16;

    return h;
}

uint32_t murmur_hash3_x86_32(const void* key, std::size_t length, uint32_t seed) {
    const uint8_t* bytes = reinterpret_cast<const uint8_t*>(key);
    const std::size_t nb_blocks = length / 4;

    uint32_t h1 = seed;

    uint32_t c1 = 0xcc9e2d51;
    uint32_t c2 = 0x1b873593;

    const uint32_t* blocks = reinterpret_cast<const uint32_t*>(bytes);
    for(int i = 0; i < nb_blocks; ++i) {
        uint32_t k1 = blocks[i];

        k1 *= c1;
        k1 = rot32(k1, 15);
        k1 *= c2;

        h1 ^= k1;
        h1 = rot32(h1, 13);
        h1 = h1 * 5 + 0xe6546b64;
    }

    const uint8_t* tail = reinterpret_cast<const uint8_t*>(bytes + nb_blocks * 4);
    uint32_t k1 = 0;
    switch(length & 3) {
        case 3: k1 ^= tail[2] << 16;
        case 2: k1 ^= tail[1] << 8;
        case 1: k1 ^= tail[0];
                k1 *= c1;
                k1 *= rot32(k1, 15);
                k1 *= c2;
                h1 ^= k1;
    }

    h1 ^= length;
    h1 = fmix32(h1);

    return h1;
}

}
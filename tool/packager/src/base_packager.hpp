#ifndef SPOOKYSUSHI_BASE_PACKAGER_HPP
#define SPOOKYSUSHI_BASE_PACKAGER_HPP

#include <package.hpp>

#include <vector>
#include <cstdint>
#include <iostream>

class UnsupportedFormat{};

struct PackagedContent {
    uint16_t type;
    uint64_t size;
    std::vector<uint8_t> data;

    PackagedContent();
    PackagedContent(uint16_t type, uint64_t size, const uint8_t* data);
};

class BasePackager {
public:
    virtual std::vector<PackagedContent> package(const std::string& filepath) = 0;
};

#endif //SPOOKYSUSHI_BASE_PACKAGER_HPP

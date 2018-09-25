#include "base_packager.hpp"

PackagedContent::PackagedContent()
: type{}
, size{}
, data{} {

}

PackagedContent::PackagedContent(uint16_t type, uint64_t size, const uint8_t* data)
: type{type}
, size{size}
, data{data + 0, data + size} {

}

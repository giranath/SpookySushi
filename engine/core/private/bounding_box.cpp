#include "bounding_box.hpp"

#include <algorithm>

namespace sushi {

bool BoundingBox::overlaps(const BoundingBox& other) const noexcept {
    return !(other.min_x > max_x
          || other.max_x < min_x
          || other.min_y > max_y
          || other.max_y < min_y
          || other.min_z > max_z
          || other.max_z < min_z);
}

BoundingBox BoundingBox::operator+(const BoundingBox& other) const noexcept {
    return BoundingBox(std::min(min_x, other.min_x),
                       std::min(min_y, other.min_y),
                       std::min(min_z, other.min_z),
                       std::max(max_x, other.max_x),
                       std::max(max_y, other.max_y),
                       std::max(max_z, other.max_z));
}

}
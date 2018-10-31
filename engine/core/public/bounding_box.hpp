#ifndef SPOOKYSUSHI_BOUNDING_BOX_HPP
#define SPOOKYSUSHI_BOUNDING_BOX_HPP

#include <types.hpp>

namespace sushi {

class BoundingBox {
    float min_x, min_y, min_z;
    float max_x, max_y, max_z;

public:
    BoundingBox() noexcept
    : min_x{}
    , min_y{}
    , min_z{}
    , max_x{}
    , max_y{}
    , max_z{} {

    }

    BoundingBox(float min_x, float min_y, float min_z, float max_x, float max_y, float max_z)
    : min_x{min_x}
    , min_y{min_y}
    , min_z{min_z}
    , max_x{max_x}
    , max_y{max_y}
    , max_z{max_z} {
        assert(min_x <= max_x);
        assert(min_y <= max_y);
        assert(min_z <= max_z);
    }

    BoundingBox(Vec3 min, Vec3 max) noexcept
    : min_x{min.x}
    , min_y{min.y}
    , min_z{min.z}
    , max_x{max.x}
    , max_y{max.y}
    , max_z{max.z} {
        assert(min.x <= max.x);
        assert(min.y <= max.y);
        assert(min.z <= max.z);
    }

    BoundingBox(Vec3 center, float half_width, float half_height, float half_depth) noexcept
    : min_x{center.x - half_width}
    , min_y{center.y - half_height}
    , min_z{center.z - half_depth}
    , max_x{center.x + half_width}
    , max_y{center.y + half_height}
    , max_z{center.z + half_depth} {

    }

    bool overlaps(const BoundingBox& other) const noexcept;
    BoundingBox operator+(const BoundingBox& other) const noexcept;

    float width() const noexcept { return max_x - min_x; }
    float height() const noexcept { return max_y - min_y; }
    float depth() const noexcept { return max_z - min_z; }
};

}

#endif

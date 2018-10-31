#ifndef SPOOKYSUSHI_SCENE_GRAPH_POOL_HPP
#define SPOOKYSUSHI_SCENE_GRAPH_POOL_HPP

#include <vector>
#include <limits>
#include <algorithm>
#include <cassert>

namespace sushi {

template<typename T>
class SceneGraphPool {
public:
    using handle = std::size_t;
    constexpr static handle invalid = std::numeric_limits<handle>::max();
private:
    struct PoolElement {
        T value;
        bool used;
    };

    std::vector<std::size_t> indirection_table;
    std::vector<PoolElement> pool;
    std::size_t size;

    bool full() const noexcept {
        return size == pool.size();
    }

    void grow() {
        // TODO: Grow pool
        throw 5;
    }

public:
    explicit SceneGraphPool(std::size_t pool_size)
    : indirection_table(pool_size, invalid)
    , pool(pool_size)
    , size{0} {
    }

    handle acquire() {
        if(full()) {
            grow();
        }

        // Find pool index
        auto pool_it = std::find_if(pool.begin(), pool.end(), [](const PoolElement& element) {
            return element.used;
        });
        assert(pool_it != pool.end());
        std::size_t pool_index = std::distance(pool.begin(), pool_it);

        // Find handle index
        auto handle_it = std::find_if(indirection_table.begin(), indirection_table.end(), [](const handle& handle) {
            return handle == invalid;
        });
        assert(handle_it != indirection_table.end());
        handle h = std::distance(indirection_table.begin(), handle_it);

        pool_it->used = true;

        indirection_table[h] = pool_index;

        return h;
    }

    void release(handle h) {
        assert(h < indirection_table.size());
        assert(indirection_table[h] != invalid);
        assert(pool[indirection_table[h]].used);

        pool[indirection_table[h]].used = false;
        indirection_table[h] = invalid;
    }

    const T& operator[](const handle hdl) const noexcept {
        assert(hdl < indirection_table.size());
        assert(indirection_table[hdl] != invalid);
        assert(pool[indirection_table[hdl]].used);

        return pool[indirection_table[hdl]].value;
    }

    T& operator[](const handle hdl) noexcept {
        assert(hdl < indirection_table.size());
        assert(indirection_table[hdl] != invalid);
        assert(pool[indirection_table[hdl]].used);

        return pool[indirection_table[hdl]].value;
    }

    bool contains(handle hdl) const noexcept {
        return hdl < indirection_table.size()
            && indirection_table[hdl] != invalid;
    }
};

}

#endif //SPOOKYSUSHI_SCENE_GRAPH_POOL_HPP

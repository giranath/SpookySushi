#ifndef SPOOKYSUSHI_INDIRECT_CONTIGUOUS_COMPONENT_STORAGE_HPP
#define SPOOKYSUSHI_INDIRECT_CONTIGUOUS_COMPONENT_STORAGE_HPP

#include "entity.hpp"

#include <vector>
#include <iterator>
#include <algorithm>
#include <limits>

namespace sushi {

template<typename ComponentType>
class IndirectContiguousComponentStorage {
public:
    using component_type = ComponentType;
    using pair_type = std::pair<const entity, ComponentType>;
    using iterator = typename std::vector<pair_type>::iterator;
    using const_iterator = typename std::vector<pair_type>::const_iterator;
    using size_type = typename std::vector<pair_type>::size_type;

private:
    static const entity_index INVALID_ENTITY_INDEX = std::numeric_limits<entity_index>::max();

    std::vector<entity_index> mapping;
    std::vector<pair_type> components;
public:

    iterator insert(const pair_type& pair);

    iterator insert(pair_type&& pair);

    // Insert multiple pairs
    template<typename InputIterator>
    void insert(InputIterator begin, InputIterator end) {
        static_assert(std::is_convertible<typename std::iterator_traits<InputIterator>::value_type, pair_type>::value, "Input iterator's value type must be convertible to a pair");
        throw 5;
    }

    // Deletion
    void erase(iterator position) {
        const entity_index to_erase_index = index(position->first);

        // Update mapping
        mapping[to_erase_index] = INVALID_ENTITY_INDEX;
        for(std::size_t i = to_erase_index + 1; i < size(); ++i) {
            --mapping[i];
        }
        components.erase(position);
    }

    void erase(iterator begin, iterator end) {
        const auto to_erase_count = std::distance(begin, end);

        // Remove all the components
        components.erase(begin, end);

        entity_index last_entity_index;
        for(; begin != end; ++begin) {
            const entity_index to_remap_index = index(begin->first);
            mapping[to_remap_index] = INVALID_ENTITY_INDEX;

            last_entity_index = to_remap_index;
        }

        for(std::size_t i = last_entity_index + 1; i < size(); ++i) {
            mapping[i] -= to_erase_count;
        }
    }

    // Access
    iterator find(entity e) noexcept {
        const entity_index e_index = index(e);

        if(e_index < mapping.size() && mapping[e_index] != INVALID_ENTITY_INDEX) {
            return std::next(components.begin(), mapping[e_index]);
        }

        return end();
    }

    const_iterator find(entity e) const noexcept {
        const entity_index e_index = index(e);

        if(e_index < mapping.size() && mapping[e_index] != INVALID_ENTITY_INDEX) {
            return std::next(components.begin(), mapping[e_index]);
        }

        return end();
    }

    // Clear
    void clear() {
        components.clear();
        mapping.clear();
    }

    // Range interface
    iterator begin() noexcept { return components.begin(); }
    iterator end() noexcept { return components.end(); }
    const_iterator begin() const noexcept { return components.begin(); }
    const_iterator end() const noexcept { return components.end(); }
    const_iterator cbegin() const noexcept { return begin(); }
    const_iterator cend() const noexcept { return end(); }

    // Size
    size_type size() const noexcept { return components.size(); }
    bool empty() const noexcept { return components.empty(); }
};

}

#endif //SPOOKYSUSHI_INDIRECT_CONTIGUOUS_COMPONENT_STORAGE_HPP

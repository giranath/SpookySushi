#ifndef SPOOKYSUSHI_VECTOR_STORAGE_HPP
#define SPOOKYSUSHI_VECTOR_STORAGE_HPP

#include "component_storage.hpp"
#include <vector>
#include <utility>
#include <iterator>

namespace sushi {

template<typename ComponentType>
class VectorStorage {
public:
    using value_type = std::pair<const Entity, ComponentType>;
    using iterator = typename std::vector<value_type>::iterator;
    using const_iterator = typename std::vector<value_type>::iterator;
private:
    std::vector<value_type> components;
public:
    std::pair<iterator, bool> insert(const value_type& value) {
        auto it = std::lower_bound(std::begin(components), std::end(components), value);

        if(it->first == value.first) {
            return std::make_pair(it, false);
        }
        else {
            return std::make_pair(components.insert(it, value), true);
        }
    }

    template<typename P>
    std::enable_if_t<std::is_constructible_v<value_type, P&&>, std::pair<iterator, bool>>
    insert(P&& value) {
        return insert(std::forward<P>(value));
    }

    iterator insert(const_iterator hint, const value_type& value) {
        // TODO:
    }

    template<typename P>
    std::enable_if_t<std::is_constructible_v<value_type, P&&>, iterator>
    insert(const_iterator hint, P&& value) {
        return insert(hint, std::forward<P>(value));
    }

    template<typename InputIterator>
    std::enable_if_t<std::is_constructible_v<value_type, typename std::iterator_traits<InputIterator>::value_type>>
    insert(InputIterator first, InputIterator last) {
        for(; first != last; ++first) {
            insert(*first);
        }
    }

    iterator begin() noexcept { return components.begin(); }
    iterator end() noexcept { return components.end(); }

    const_iterator begin() const noexcept { return components.begin(); }
    const_iterator end() const noexcept { return components.end(); }

    const_iterator cbegin() const noexcept { return components.begin(); }
    const_iterator cend() const noexcept { return components.end(); }
};

template<typename ComponentType>
struct component_storage_traits<VectorStorage<ComponentType>> {
    using component_type = ComponentType;
    using iterator = typename std::vector<std::pair<const Entity, ComponentType>>::iterator;
    using const_iterator = typename std::vector<std::pair<const Entity, ComponentType>>::const_iterator;
};

}

#endif //SPOOKYSUSHI_VECTOR_STORAGE_HPP

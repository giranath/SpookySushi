#ifndef SPOOKYSUSHI_DICTIONARY_COMPONENT_STORAGE_HPP
#define SPOOKYSUSHI_DICTIONARY_COMPONENT_STORAGE_HPP

#include "entity.hpp"
#include "component_storage_traits.hpp"
#include <map>
#include <iterator>
#include <type_traits>

namespace sushi {

template<typename ComponentType>
class DictionaryComponentStorage {
    std::map<entity, ComponentType> container;
public:
    using component_type = ComponentType;
    using iterator = typename std::map<entity, ComponentType>::iterator;
    using const_iterator = typename std::map<entity, ComponentType>::const_iterator;
    using pair_type = std::pair<const entity, component_type>;
    using size_type = typename std::map<entity, ComponentType>::size_type;

    // Insertion
    iterator insert(const pair_type& pair) { return container.insert(pair); }

    iterator insert(pair_type&& pair) { return container.insert(std::move(pair)); }

    // Insert multiple pairs
    template<typename InputIterator>
    void insert(InputIterator begin, InputIterator end) {
        static_assert(std::is_convertible<typename std::iterator_traits<InputIterator>::value_type, pair_type>::value, "Input iterator's value type must be convertible to a pair");

        container.insert(begin, end);
    }

    // Deletion
    void erase(iterator position) { container.erase(position); }

    void erase(iterator begin, iterator end) { container.erase(begin, end); }

    // Access
    iterator find(entity e) noexcept { return container.find(e); }

    const_iterator find(entity e) const noexcept { return container.find(e); }

    // Clear
    void clear() { container.clear(); }

    // Range interface
    iterator begin() noexcept { return container.begin(); }
    iterator end() noexcept { return container.end(); }
    const_iterator begin() const noexcept { return container.begin(); }
    const_iterator end() const noexcept { return container.end(); }
    const_iterator cbegin() const noexcept { return begin(); }
    const_iterator cend() const noexcept { return end(); }

    // Size
    size_type size() const noexcept { return container.size(); }
    bool empty() const noexcept { return container.empty(); }
};

template<typename ComponentType>
struct component_storage_traits<DictionaryComponentStorage<ComponentType>> {
    using component_type = typename DictionaryComponentStorage<ComponentType>::component_type;
};

}

#endif //SPOOKYSUSHI_DICTIONARY_COMPONENT_STORAGE_HPP

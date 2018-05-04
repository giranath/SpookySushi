#ifndef SPOOKYSUSHI_NULL_STORAGE_HPP
#define SPOOKYSUSHI_NULL_STORAGE_HPP

#include "entity.hpp"
#include "storage_traits.hpp"
#include <set>
#include <iterator>

namespace sushi { namespace ecs {

struct null_component {};

class null_storage {
    std::set<entity> entities;
public:
    class iterator : public std::iterator<std::forward_iterator_tag, std::pair<const entity, null_component>> {
        friend null_storage;
        std::set<entity>::iterator it;
        const null_storage* owner;
        using pair = std::pair<const entity, null_component>;
        alignas(pair) char raw_pair[sizeof(pair)];

        void update_pair();

        iterator(const null_storage* owner, std::set<entity>::iterator it) noexcept;
    public:
        iterator() noexcept;

        iterator& operator++();
        iterator operator++(int);

        bool operator==(const iterator& other) noexcept;
        bool operator!=(const iterator& other) noexcept;

        reference operator*() noexcept;
        pointer operator->() noexcept;
    };

    using const_iterator = iterator;
    using component_type = null_component;

    void clear();
    void insert(entity e);

    // Search for entity
    iterator find(entity e) noexcept;
    const_iterator find(entity e) const noexcept;

    // Iterators
    iterator begin() noexcept;
    const_iterator begin() const noexcept;
    const_iterator cbegin() const noexcept;

    iterator end() noexcept;
    const_iterator end() const noexcept;
    const_iterator cend() const noexcept;
};

template<>
struct storage_traits<null_storage> {
    using component_type = typename null_storage::component_type;
    using iterator = typename null_storage::iterator;
    using const_iterator = typename null_storage::const_iterator;
};

}}

#endif

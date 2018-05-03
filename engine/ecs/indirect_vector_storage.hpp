#ifndef SPOOKYSUSHI_INDIRECT_VECTOR_STORAGE_HPP
#define SPOOKYSUSHI_INDIRECT_VECTOR_STORAGE_HPP

#include "entity.hpp"
#include "storage_traits.hpp"

#include <limits>
#include <cstddef>
#include <vector>
#include <iterator>

namespace sushi { namespace ecs {

template<typename Component>
class indirect_vector_storage {
public:
    using component_type = Component;

    class iterator : public std::iterator<std::forward_iterator_tag, std::pair<const entity, component_type&>> {
        friend indirect_vector_storage;
        entity cur_entity;
        indirect_vector_storage* owner;
        alignas(std::pair<const entity, component_type&>) char raw_pair[sizeof(std::pair<const entity, component_type&>)];

        iterator(indirect_vector_storage* owner, entity e)
        : cur_entity{e}
        , owner{owner} {
            update_pair();
        }

        void update_pair() {
            new(raw_pair) std::pair<const entity, component_type&>(cur_entity, owner->components[owner->component_indices[cur_entity]]);
        }
    public:
        iterator& operator++() {
            // Find next element
            auto it = std::find_if_not(std::begin(owner->component_indices) + cur_entity + 1,
                                       std::end(owner->component_indices),
                                       [](const std::size_t i) { return i == INVALID_COMPONENT_INDEX; });

            if(it != std::end(owner->component_indices)) {
                cur_entity = static_cast<entity>(std::distance(std::begin(owner->component_indices), it));
                update_pair();
            }
            else {
                cur_entity = static_cast<entity>(owner->component_indices.size());
            }

            return *this;
        }

        iterator operator++(int) {
            iterator old = *this;

            ++(*this);

            return old;
        }

        bool operator==(const iterator& other) const noexcept {
            return owner == other.owner && cur_entity == other.cur_entity;
        }

        bool operator!=(const iterator& other) const noexcept {
            return owner != other.owner || cur_entity != other.cur_entity;
        }

        std::pair<const entity, component_type&>& operator*() {
            return *reinterpret_cast<std::pair<const entity, component_type&>*>(raw_pair);
        }

        std::pair<const entity, component_type&>* operator->() {
            return reinterpret_cast<std::pair<const entity, component_type&>*>(raw_pair);
        }
    };
    friend iterator;

    class const_iterator : public std::iterator<std::forward_iterator_tag, std::pair<const entity, const component_type&>> {
        friend indirect_vector_storage;
        entity cur_entity;
        indirect_vector_storage* owner;
        alignas(std::pair<const entity, const component_type&>) char raw_pair[sizeof(std::pair<const entity, const component_type&>)];

        const_iterator(indirect_vector_storage* owner, entity e)
        : cur_entity{e}
        , owner{owner} {
            update_pair();
        }

        void update_pair() {
            new(raw_pair) std::pair<const entity, const component_type&>(cur_entity, owner->components[owner->component_indices[cur_entity]]);
        }
    public:
        const_iterator& operator++() {
            // Find next element
            auto it = std::find_if_not(std::begin(owner->component_indices) + cur_entity + 1,
                                       std::end(owner->component_indices),
                                       [](const std::size_t i) { return i == INVALID_COMPONENT_INDEX; });

            if(it != std::end(owner->component_indices)) {
                cur_entity = static_cast<entity>(std::distance(std::begin(owner->component_indices), it));
                update_pair();
            }
            else {
                cur_entity = static_cast<entity>(owner->component_indices.size());
            }

            return *this;
        }

        const_iterator operator++(int) {
            const_iterator old = *this;

            ++(*this);

            return old;
        }

        bool operator==(const const_iterator& other) const noexcept {
            return owner == other.owner && cur_entity == other.cur_entity;
        }

        bool operator!=(const const_iterator& other) const noexcept {
            return owner != other.owner || cur_entity != other.cur_entity;
        }

        std::pair<const entity, const component_type&>& operator*() {
            return *reinterpret_cast<std::pair<const entity, const component_type&>*>(raw_pair);
        }

        std::pair<const entity, const component_type&>* operator->() {
            return reinterpret_cast<std::pair<const entity, const component_type&>*>(raw_pair);
        }
    };
    friend const_iterator;

private:
    constexpr static const std::size_t INVALID_COMPONENT_INDEX = std::numeric_limits<std::size_t>::max();

    std::vector<std::size_t> component_indices;
    std::vector<component_type> components;

public:
    void clear() {
        std::fill(std::begin(component_indices), std::end(component_indices), INVALID_COMPONENT_INDEX);
        components.clear();
    }

    void insert(entity e, const component_type& component) {
        // Udpate storage
        if(e < component_indices.size()) {
            // New entity
            if(component_indices[e] == INVALID_COMPONENT_INDEX) {
                component_indices[e] = components.size();
                components.push_back(component);
            }
            // Update old component with new one
            else {
                components[component_indices[e]] = component;
            }
        }
        // Grow the storage
        else {
            component_indices.resize(e, INVALID_COMPONENT_INDEX);
            component_indices.push_back(components.size());
            components.push_back(component);
        }
    }

    void insert(entity e, component_type&& component) {
        // Udpate storage
        if(e < component_indices.size()) {
            // New entity
            if(component_indices[e] == INVALID_COMPONENT_INDEX) {
                component_indices[e] = components.size();
                components.push_back(std::move(component));
            }
                // Update old component with new one
            else {
                components[component_indices[e]] = std::move(component);
            }
        }
            // Grow the storage
        else {
            component_indices.resize(e, INVALID_COMPONENT_INDEX);
            component_indices.push_back(components.size());
            components.push_back(std::move(component));
        }
    }

    template<typename... Args>
    void emplace(entity e, Args&&... args) {
        if(e < component_indices.size()) {
            // New entity
            if(component_indices[e] == INVALID_COMPONENT_INDEX) {
                component_indices[e] = components.size();
                components.emplace_back(std::forward<Args>(args)...);
            }
                // Update old component with new one
            else {
                components.emplace(components.begin() + component_indices[e], std::forward<Args>(args)...);
            }
        }
            // Grow the storage
        else {
            component_indices.resize(e, INVALID_COMPONENT_INDEX);
            component_indices.push_back(components.size());
            components.emplace_back(std::forward<Args>(args)...);
        }
    }

    iterator find(entity e) noexcept {
        if(e < component_indices.size() && component_indices[e] != INVALID_COMPONENT_INDEX) {
            return iterator{this, e};
        }

        return end();
    }

    const_iterator find(entity e) const noexcept {
        if(e < component_indices.size() && component_indices[e] != INVALID_COMPONENT_INDEX) {
            return const_iterator{this, e};
        }

        return end();
    }

    // Iterators
    iterator begin() noexcept {
        auto it = std::find_if_not(std::begin(component_indices),
                                   std::end(component_indices),
                                   [](const std::size_t i) { return i == INVALID_COMPONENT_INDEX; });

        if(it != std::end(component_indices)) {
            return iterator{this, static_cast<entity>(std::distance(std::begin(component_indices), it))};
        }

        return end();
    }

    const_iterator begin() const noexcept {
        auto it = std::find_if_not(std::begin(component_indices),
                                   std::end(component_indices),
                                   [](const std::size_t i) { return i == INVALID_COMPONENT_INDEX; });

        if(it != std::end(component_indices)) {
            return const_iterator{this, static_cast<entity>(std::distance(std::begin(component_indices), it))};
        }

        return end();
    }

    const_iterator cbegin() const noexcept {
        auto it = std::find_if_not(std::begin(component_indices),
                                   std::end(component_indices),
                                   [](const std::size_t i) { return i == INVALID_COMPONENT_INDEX; });

        if(it != std::end(component_indices)) {
            return const_iterator{this, static_cast<entity>(std::distance(std::begin(component_indices), it))};
        }

        return end();
    }

    iterator end() noexcept {
        return iterator{this, static_cast<entity>(component_indices.size())};
    }

    const_iterator end() const noexcept {
        return const_iterator{this, static_cast<entity>(component_indices.size())};
    }

    const_iterator cend() const noexcept {
        return const_iterator{this, static_cast<entity>(component_indices.size())};
    }
};

template<typename Component>
struct storage_traits<indirect_vector_storage<Component>> {
    using component_type = typename indirect_vector_storage<Component>::component_type;
    using iterator = typename indirect_vector_storage<Component>::iterator;;
    using const_iterator = typename indirect_vector_storage<Component>::const_iterator;
};

}}

#endif //SPOOKYSUSHI_INDIRECT_VECTOR_STORAGE_HPP

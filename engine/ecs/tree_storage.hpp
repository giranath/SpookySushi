#ifndef SPOOKYSUSHI_TREE_STORAGE_HPP
#define SPOOKYSUSHI_TREE_STORAGE_HPP

#include "entity.hpp"
#include <map>

namespace sushi { namespace ecs {

template<typename Component>
class tree_storage {
public:
    using component_type = Component;
    using iterator = typename std::map<entity, component_type>::iterator;
    using const_iterator = typename std::map<entity, component_type>::const_iterator;
private:
    std::map<entity, component_type> components;
public:
    void clear() {
        components.clear();
    }

    void insert(entity e, const component_type& component) {
        components.insert(std::make_pair(e, component));
    }

    void insert(entity e, component_type&& component) {
        component.insert(std::make_pair(e, std::move(component)));
    }

    template<typename... Args>
    void emplace(entity e, Args&&... args) {
        components.emplace(e, std::forward<Args>(args)...);
    }

    // Search for an entity
    iterator find(entity e) noexcept {
        return components.find(e);
    }

    const_iterator find(entity e) const noexcept {
        return components.find(e);
    }

    // Iterators
    iterator begin() noexcept {
        return components.begin();
    }

    const_iterator begin() const noexcept {
        return components.begin();
    }

    const_iterator cbegin() const noexcept {
        return components.cbegin();
    }

    iterator end() noexcept {
        return components.end();
    }

    const_iterator end() const noexcept {
        return components.end();
    }

    const_iterator cend() const noexcept {
        return components.cend();
    }
};

}}

#endif

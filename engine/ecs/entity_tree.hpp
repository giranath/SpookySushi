#ifndef SPOOKYSUSHI_ENTITY_TREE_HPP
#define SPOOKYSUSHI_ENTITY_TREE_HPP

#include "entity.hpp"
#include <vector>
#include <memory>
#include <iterator>

namespace sushi { namespace ecs {

class entity_tree {
    struct node {
        node* parent;
        std::vector<node*> children;
        entity id;

        explicit node(entity id) noexcept;
        node(entity id, node* parent) noexcept;
        ~node() noexcept;
    };

    std::unique_ptr<node> root = {};
    entity next_entity = 0;
public:
    static const constexpr entity ROOT_ENTITY = 0;
    class iterator : public std::iterator<std::forward_iterator_tag,
                                          const entity,
                                          entity,
                                          const entity*,
                                          entity> {
        friend entity_tree;
        node* current_node = nullptr;

        constexpr iterator(node* n) noexcept
        : current_node{n} {

        }
    public:
        iterator() noexcept = default;
        iterator& operator++() noexcept;
        iterator operator++(int) noexcept;
        bool operator==(const iterator& other) const noexcept;
        bool operator!=(const iterator& other) const noexcept;

        pointer operator->() const noexcept;
        reference operator*() const noexcept;
    };

public:
    entity_tree();
    iterator begin() const noexcept;
    iterator end() const noexcept;

    iterator make();
    iterator make(iterator parent);
    iterator find(entity e) const noexcept;

    void remove(iterator it) noexcept;
    void clear() noexcept;

    iterator next_sibling(iterator it) const noexcept;
    iterator parent(iterator it) const noexcept;
};

}}

#endif //SPOOKYSUSHI_ENTITY_TREE_HPP

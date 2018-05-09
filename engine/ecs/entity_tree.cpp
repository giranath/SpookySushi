#include "entity_tree.hpp"

#include <algorithm>

namespace sushi { namespace ecs {

entity_tree::node::node(entity id) noexcept
: parent{nullptr}
, children{}
, id{id} {

}

entity_tree::node::node(entity id, node* parent) noexcept
: parent{parent}
, children{}
, id{id} {

}

entity_tree::node::~node() noexcept {
    std::for_each(std::begin(children), std::end(children), [](node* n) {
        delete n;
    });
}

entity_tree::iterator& entity_tree::iterator::operator++() noexcept {
    if(current_node->children.empty()) {
        node* initial_node = current_node;
        node* traversal_node = current_node;
        // Move up
        do {
            node* parent = traversal_node->parent;

            if(parent) {
                auto it = std::find(std::begin(parent->children), std::end(parent->children),
                                    traversal_node);
                std::size_t sibling_index = std::distance(std::begin(parent->children), it) + 1;

                if (sibling_index < parent->children.size()) {
                    // Next sibling
                    current_node = parent->children[sibling_index];
                } else {
                    // On doit remonter
                    traversal_node = traversal_node->parent;
                }
            }
            else {
                current_node = nullptr;
            }
        } while(current_node == initial_node);
    }
    else {
        current_node = current_node->children.front();
    }

    return *this;
}

entity_tree::iterator entity_tree::iterator::operator++(int) noexcept {
    iterator old = *this;

    ++(*this);

    return old;
}

bool entity_tree::iterator::operator==(const iterator& other) const noexcept {
    return current_node == other.current_node;
}

bool entity_tree::iterator::operator!=(const iterator& other) const noexcept {
    return current_node != other.current_node;
}

entity_tree::iterator::pointer entity_tree::iterator::operator->() const noexcept {
    return &current_node->id;
}

entity_tree::iterator::reference entity_tree::iterator::operator*() const noexcept {
    return current_node->id;
}

entity_tree::entity_tree()
: root{std::make_unique<node>(ROOT_ENTITY)}
, next_entity{1}{

}

entity_tree::iterator entity_tree::begin() const noexcept {
    return iterator{root.get()};
}

entity_tree::iterator entity_tree::end() const noexcept {
    return iterator{};
}

entity_tree::iterator entity_tree::make() {
    entity next = next_entity++;

    root->children.push_back(new node{next, root.get()});

    return iterator{root->children.back()};
}

entity_tree::iterator entity_tree::make(iterator parent) {
    entity next = next_entity++;

    parent.current_node->children.push_back(new node{next, parent.current_node});

    return iterator{parent.current_node->children.back()};
}

void entity_tree::remove(iterator it) noexcept {
    if(it.current_node && it.current_node->parent) {
        auto child_it = std::find(std::begin(it.current_node->parent->children),
                                  std::end(it.current_node->parent->children),
                                  it.current_node);

        if(child_it != std::end(it.current_node->parent->children)) {
            std::swap(*child_it, it.current_node->parent->children.back());
            it.current_node->parent->children.pop_back();
        }
    }
}

void entity_tree::clear() noexcept {
    root->children.clear();
}

entity_tree::iterator entity_tree::find(entity e) const noexcept {
    return std::find(begin(), end(), e);
}

entity_tree::iterator entity_tree::next_sibling(iterator it) const noexcept {
    if(it.current_node && it.current_node->parent) {
        auto it_sibling = std::find(std::begin(it.current_node->parent->children),
                                    std::end(it.current_node->parent->children),
                                    it.current_node);
        if (it_sibling != std::end(it.current_node->parent->children)) {
            return iterator{it.current_node->parent->children[std::distance(std::begin(it.current_node->parent->children),
                                                                            it_sibling) + 1]};
        }
    }

    return end();
}

entity_tree::iterator entity_tree::parent(iterator it) const noexcept {
    if(it.current_node && it.current_node->parent) {
        return iterator{it.current_node->parent};
    }

    return end();
}

}}
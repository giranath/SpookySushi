#ifndef SPOOKYSUSHI_SCENE_GRAPH_HPP
#define SPOOKYSUSHI_SCENE_GRAPH_HPP

#include "scene_graph_pool.hpp"

#include <event.hpp>
#include <entity.hpp>
#include <bounding_box.hpp>

#include <vector>
#include <algorithm>
#include <iterator>
#include <functional>

namespace sushi {


class SceneGraph {
public:
    using node_handle = std::size_t;
    using entities_destroyed_event = event<std::vector<Entity>&&>;
    using query_bounding_box_fn = std::function<BoundingBox(Entity)>;

    class Node {
        node_handle parent_;
        std::vector<node_handle> children_;
        Entity associated_entity_;
        mutable BoundingBox cached_bounds;
        mutable bool bounds_dirty;
    public:
        using iterator = std::vector<node_handle>::iterator;
        using const_iterator = std::vector<node_handle>::const_iterator;

        Node() noexcept
        : parent_{invalid_handle}
        , children_{}
        , associated_entity_{Entity::invalid}
        , bounds_dirty{true} {

        }

        Node(Entity entity, node_handle parent) noexcept
        : parent_{invalid_handle}
        , children_{}
        , associated_entity_{entity}
        , bounds_dirty{true}{

        }

        explicit Node(node_handle parent) noexcept
        : parent_{parent}
        , children_{}
        , associated_entity_{}
        , bounds_dirty{true} {

        }

        const node_handle parent() const noexcept { return parent_; }

        Entity entity() const noexcept { return associated_entity_; }
        Entity& entity() noexcept { return associated_entity_; }

        iterator begin() noexcept { return children_.begin(); }
        iterator end() noexcept { return children_.end(); }
        const_iterator begin() const noexcept { return children_.begin(); }
        const_iterator end() const noexcept { return children_.end(); }
        const_iterator cbegin() const noexcept { return begin(); }
        const_iterator cend() const noexcept { return end(); }

        void remove(iterator it) {
            std::swap(*it, children_.back());
            children_.pop_back();
        }

        BoundingBox bounds(const SceneGraphPool<Node>& pool, const query_bounding_box_fn& query_bounding_box) const noexcept;

        void dirty_bounds() noexcept { bounds_dirty = true; }
    };
private:
    Node root;
    std::size_t size_;
    SceneGraphPool<Node> pool;
    query_bounding_box_fn query_bounding_box;
public:
    entities_destroyed_event entities_destroyed;

    static const node_handle invalid_handle;

    SceneGraph(std::size_t pool_size = 1024);
    std::size_t size() const noexcept;
    bool empty() const noexcept;

    // Create an entity node
    node_handle make_entity_node(Entity entity);
    node_handle make_entity_node(Entity entity, node_handle parent);
    node_handle make_group_node();
    node_handle make_group_node(node_handle parent);

    void dirty_bounds(node_handle node);

    // Destroy a node
    void destroy(node_handle node);

    // Destroy all tree
    void clear();

    void set_bounding_box_query(query_bounding_box_fn query) noexcept { query_bounding_box = query; }

    // Return every entities under a node
    template<typename OutIt>
    OutIt find_entity_tree(node_handle root_handle, OutIt out) const noexcept {
        std::vector<node_handle> open_nodes;
        open_nodes.reserve(16);

        assert(pool.contains(root_handle));

        open_nodes.push_back(root_handle);

        while(!open_nodes.empty()) {
            const Node& node = pool[open_nodes.back()];
            open_nodes.pop_back();

            if(node.entity()) {
                out = node.entity();
                ++out;
            }

            open_nodes.insert(open_nodes.end(), node.begin(), node.end());
        }

        return out;
    }

    // Return every entities overlapping with box
    template<typename OutIt>
    OutIt find_overlapping_entities(const BoundingBox& box, OutIt out) const noexcept {
        std::vector<node_handle> open_nodes;
        open_nodes.reserve(std::max(size() / 3, static_cast<std::size_t>(64)));
        open_nodes.insert(open_nodes.end(), root.begin(), root.end());

        while(open_nodes.empty()) {
            const node_handle handle = open_nodes.back();
            open_nodes.pop_back();

            assert(pool.contains(handle));

            const Node& node = pool[handle];

            if(node.bounds(pool, query_bounding_box).overlaps(box)) {
                if(node.entity()) {
                    out = node.entity();
                    ++out;
                }

                open_nodes.insert(open_nodes.end(), node.begin(), node.end());
            }
        }

        return out;
    }

};

}

#endif

#include "scene_graph.hpp"

namespace sushi {

BoundingBox SceneGraph::Node::bounds(const SceneGraphPool<Node>& pool, const query_bounding_box_fn& query_bounding_box) const noexcept {
    if(bounds_dirty) {
        BoundingBox box;
        if(associated_entity_) {
            box = query_bounding_box(associated_entity_);
        }

        // Calculate box for each child
        std::for_each(std::begin(children_), std::end(children_), [&box, &pool, &query_bounding_box](const node_handle child) {
            assert(pool.contains(child));
            box = box + pool[child].bounds(pool, query_bounding_box);
        });

        cached_bounds = box;
        bounds_dirty = false;

        return box;
    }
    else {
        return cached_bounds;
    }
}

const SceneGraph::node_handle SceneGraph::invalid_handle = std::numeric_limits<node_handle>::max();

BoundingBox default_bounding_box_query(Entity) {
    return BoundingBox{};
}

SceneGraph::SceneGraph(std::size_t pool_size)
: size_{}
, pool{pool_size}
, query_bounding_box{default_bounding_box_query} {
}

std::size_t SceneGraph::size() const noexcept {
    return size_;
}

bool SceneGraph::empty() const noexcept {
    return size_ == 0;
}

SceneGraph::node_handle SceneGraph::make_entity_node(Entity entity) {
    return make_entity_node(entity, invalid_handle);
}

SceneGraph::node_handle SceneGraph::make_entity_node(Entity entity, node_handle parent) {
    auto new_hdl = pool.acquire();
    pool[new_hdl] = Node(entity, parent);

    ++size_;

    return new_hdl;
}

SceneGraph::node_handle SceneGraph::make_group_node() {
    return make_group_node(invalid_handle);
}

SceneGraph::node_handle SceneGraph::make_group_node(node_handle parent) {
    auto new_hdl = pool.acquire();
    pool[new_hdl] = Node(parent);

    ++size_;

    return new_hdl;
}

void SceneGraph::dirty_bounds(node_handle node) {
    while(node != invalid_handle) {
        if (pool.contains(node)) {
            Node &element = pool[node];
            element.dirty_bounds();

            node = element.parent();
        }
        else {
            node = invalid_handle;
        }
    }
}

// Destroy a node
void SceneGraph::destroy(node_handle node) {
    std::vector<Entity> destroyed_entities;

    if(pool.contains(node)) {
        const Node& element_to_destroy = pool[node];
        destroyed_entities.push_back(element_to_destroy.entity());

        // remove from parent
        if(element_to_destroy.parent() != invalid_handle) {
            Node& parent_node = pool[element_to_destroy.parent()];

            auto it = std::find(parent_node.begin(), parent_node.end(), node);
            assert(it != parent_node.end());
            parent_node.remove(it);
        }

        pool.release(node);

        std::vector<Node*> to_remove_elements;
        std::vector<node_handle> to_remove_handles;
        std::for_each(element_to_destroy.begin(), element_to_destroy.end(), [this, &to_remove_elements, &to_remove_handles](const node_handle& handle) {
            to_remove_elements.push_back(&pool[handle]);
            to_remove_handles.push_back(handle);
        });

        while(to_remove_elements.empty()) {
            Node* to_remove = to_remove_elements.back();
            to_remove_elements.pop_back();
            const node_handle handle = to_remove_handles.back();
            to_remove_handles.pop_back();

            if(to_remove->entity()) {
                destroyed_entities.push_back(to_remove->entity());
            }

            std::for_each(to_remove->begin(), to_remove->end(), [this, &to_remove_elements, &to_remove_handles](const node_handle& child_handle) {
                to_remove_elements.push_back(&pool[child_handle]);
                to_remove_handles.push_back(child_handle);
            });

            pool.release(handle);
        }
    }

    // Delegate what to do when an entity is destroyed
    entities_destroyed.unlocked_call(std::move(destroyed_entities));
}

// Destroy all tree
void SceneGraph::clear() {
    ++size_ = 0;
}

}
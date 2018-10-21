#ifndef SPOOKYSUSHI_OPENGL_DEBUG_SHAPE_RENDERER_HPP
#define SPOOKYSUSHI_OPENGL_DEBUG_SHAPE_RENDERER_HPP

#include "debug_shapes.hpp"
#include "debug_shape_traits.hpp"

#include <types.hpp>
#include <vertex_array.hpp>
#include <buffer.hpp>

#include <vector>
#include <algorithm>
#include <iterator>
#include <mutex>

namespace sushi {

constexpr GLenum to_opengl_primitive(debug_shape_primitive primitive) {
    switch(primitive) {
        case debug_shape_primitive::Lines:
            return GL_LINES;
        case debug_shape_primitive::Triangles:
            return GL_TRIANGLES;
        default:
            return 0;
    }
}

template<typename ShapeType>
class OpenGLDebugShapeRenderer {
    std::vector<ShapeType> shapes;
    gl::vertex_array vao;
    gl::buffer vertices;
    gl::buffer colors;

    std::size_t vertice_count;
    std::size_t max;

    auto find_first_dead_shape() const noexcept {
        return std::lower_bound(std::begin(shapes), std::end(shapes), false, [](const ShapeType& shape, bool is_dead) {
            return shape.is_dead() == is_dead;
        });
    }

    void sort() {
        std::sort(std::begin(shapes), std::end(shapes));
    }

    bool full() const noexcept {
        return shapes.size() == max;
    }

public:
    explicit OpenGLDebugShapeRenderer(std::size_t max_count)
    : shapes{}
    , vao{}
    , vertices{}
    , colors{}
    , vertice_count{0}
    , max{max_count} {

    }

    void batch_shapes() {
        std::vector<Vec3> vertices_data;
        std::vector<Vec3> colors_data;

        vertices_data.reserve(debug_shape_traits<ShapeType>::vertice_count);
        colors_data.reserve(debug_shape_traits<ShapeType>::vertice_count);

        std::for_each(std::begin(shapes), std::end(shapes), [&vertices_data, &colors_data](const ShapeType& shape) {
            shape.construct(vertices_data, colors_data);
        });

        vertice_count = vertices_data.size();
        gl::bind(gl::buffer_bind<GL_ARRAY_BUFFER>(vertices));
        glBufferSubData(GL_ARRAY_BUFFER, 0, vertice_count * sizeof(Vec3), vertices_data.data());

        gl::bind(gl::buffer_bind<GL_ARRAY_BUFFER>(colors));
        glBufferSubData(GL_ARRAY_BUFFER, 0, vertice_count * sizeof(Vec3), colors_data.data());
    }

    void init() {
        shapes.reserve(max);

        vao = gl::vertex_array::make();
        vertices = gl::buffer::make();
        colors = gl::buffer::make();

        gl::bind(gl::buffer_bind<GL_ARRAY_BUFFER>(vertices));
        glBufferData(GL_ARRAY_BUFFER, max * debug_shape_traits<ShapeType>::vertice_count, nullptr, GL_DYNAMIC_DRAW);

        gl::bind(gl::buffer_bind<GL_ARRAY_BUFFER>(colors));
        glBufferData(GL_ARRAY_BUFFER, max * debug_shape_traits<ShapeType>::vertice_count, nullptr, GL_DYNAMIC_DRAW);

        gl::bind(vao);
        glEnableVertexAttribArray(0);
        glEnableVertexAttribArray(1);

        gl::bind(gl::buffer_bind<GL_ARRAY_BUFFER>(vertices));
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, nullptr);

        gl::bind(gl::buffer_bind<GL_ARRAY_BUFFER>(colors));
        glVertexAttribPointer(1, 3, GL_FLOAT, GL_TRUE, 0, nullptr);
    }

    void render() {
        gl::bind(vao);

        assert(vertice_count < std::numeric_limits<GLsizei>::max());
        glDrawArrays(to_opengl_primitive(debug_shape_traits<ShapeType>::primitive), 0, static_cast<GLsizei>(vertice_count));
    }

    template<typename... Args>
    void emplace(Args&&... arguments) {
        if(full()) throw std::out_of_range{"too many queued shapes"};
        shapes.emplace_back(std::forward<Args>(arguments)...);
    }

    void update(uint32_t dt) {
        std::for_each(std::begin(shapes), std::end(shapes), [dt](ShapeType& shape) {
            if(shape.cooldown > dt) {
                shape.cooldown -= dt;
            }
            else {
                shape.cooldown = 0;
            }
        });
    }

    void collect_garbages() {
        sort();

        auto it = find_first_dead_shape();

        if(it != std::end(shapes) && it->is_dead()) {
            shapes.erase(it, std::end(shapes));
        }
    }
};

}

#endif

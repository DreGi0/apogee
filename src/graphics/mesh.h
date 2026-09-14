//
// Created by gio on 9/13/26.
//

#ifndef APOGEE_MESH_H
#define APOGEE_MESH_H

// ========== IMPORTS ==========
#include <cstddef>
#include <glad/gl.h>

namespace Apogee {
    class Mesh {
    public:
        Mesh(const float* vertices, size_t vertexCount);

        Mesh(const Mesh&) = delete;
        Mesh& operator=(const Mesh&) = delete;

        Mesh(Mesh&& other) noexcept;
        Mesh& operator=(Mesh&& other) noexcept;

        void draw() const;

        ~Mesh();

    private:
        GLsizei vertexCount = 0;
        GLuint vao = 0;
        GLuint vbo = 0;
    };
} // Apogee

#endif //APOGEE_MESH_H

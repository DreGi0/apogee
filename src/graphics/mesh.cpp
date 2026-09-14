//
// Created by gio on 9/13/26.
//

#include <glad/gl.h>

#include "mesh.h"

namespace Apogee {
    Mesh::Mesh(const float *vertices, size_t vertexCount) {
        this->vertexCount = vertexCount;

        // ----- RESERVE IDS -----
        glGenVertexArrays(1, &vao);
        glGenBuffers(1, &vbo);

        // ----- ENABLE VAO THEN VBO -----
        glBindVertexArray(vao);
        glBindBuffer(GL_ARRAY_BUFFER, vbo);

        // ----- RAM TO CPU -----
        GLsizeiptr bufferSize =  vertexCount * 6 * sizeof(float);
        glBufferData(GL_ARRAY_BUFFER, bufferSize, vertices, GL_STATIC_DRAW);

        // ----- ATTRIBUTES -----
        // ___ POSITIONS ___
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float),
            reinterpret_cast<void*>(0));
        glEnableVertexAttribArray(0);

        // ___ COLOR ___
        glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float),
            reinterpret_cast<void*>(3 * sizeof(float)));
        glEnableVertexAttribArray(1);

        // # bind vertex
        glBindVertexArray(0);
    }

    Mesh::Mesh(Mesh &&other) noexcept :
    vertexCount(other.vertexCount),
    vao(other.vao),
    vbo(other.vbo) {
        other.vertexCount = 0;
        other.vbo = 0;
        other.vao = 0;
    }

    Mesh & Mesh::operator=(Mesh &&other) noexcept {
        if (this == &other) return *this;

        // # Clear current
        glDeleteVertexArrays(1, &vao);
        glDeleteBuffers(1, &vbo);

        // # Get the other values
        vertexCount = other.vertexCount;
        vao = other.vao;
        vbo = other.vbo;

        // # Clear other
        other.vertexCount = 0;
        other.vao = 0;
        other.vbo = 0;

        return *this;
    }

    void Mesh::draw() const {
        glBindVertexArray(vao);
        glDrawArrays(GL_TRIANGLES, 0, vertexCount);
    }

    Mesh::~Mesh() {
        glDeleteVertexArrays(1, &vao);
        glDeleteBuffers(1, &vbo);
    }
} // Apogee
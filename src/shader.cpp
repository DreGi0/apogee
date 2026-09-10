//
// Created by gio on 9/10/26.
//

#include "shader.h"

#include <fstream>
#include <sstream>
#include <stdexcept>

namespace Apogee {
    Shader::Shader(const std::string& vertPath, const std::string& fragPath) {
        const GLuint vs = compileShader(GL_VERTEX_SHADER, readFile(vertPath).c_str());
        if (vs == 0) {
            throw std::runtime_error("Failed to compile vertex shader");
        }

        const GLuint fs = compileShader(GL_FRAGMENT_SHADER, readFile(fragPath).c_str());
        if (fs == 0) {
            glDeleteShader(vs); // avoid GPU garbage
            throw std::runtime_error("Failed to compile fragment shader");
        }

        const GLuint program = glCreateProgram();
        glAttachShader(program, vs);
        glAttachShader(program, fs);

        glLinkProgram(program);

        GLint success = 0;
        glGetProgramiv(program, GL_LINK_STATUS, &success);

        if (!success) {
            GLint logLength = 0;
            glGetProgramiv(program, GL_INFO_LOG_LENGTH, &logLength);

            std::string log(static_cast<size_t>(logLength), '\0');
            glGetProgramInfoLog(program, logLength, nullptr, log.data());

            fprintf(stderr, "[shader] failed to link program:\n%s\n", log.c_str());
        }

        glDeleteShader(vs);
        glDeleteShader(fs);

        programId = program;
    }

    Shader::Shader(Shader &&other) noexcept {
        programId = other.programId;
        other.programId = 0;
    }

    Shader & Shader::operator=(Shader &&other) noexcept {
        if (this == &other)
            return *this;

        glDeleteProgram(programId);
        programId = other.programId;
        other.programId = 0;

        return *this;
    }

    Shader::~Shader() {
        glDeleteProgram(programId);
    }
    std::string Shader::readFile(const std::string& path) {
        std::ifstream file(path);

        if (!file.is_open()) {
            throw std::runtime_error("Failed to open file at path: " + path);
        }

        std::stringstream buffer;
        buffer << file.rdbuf();

        return buffer.str();
    }

    GLint Shader::getUniformLocation(const std::string &name) const {
        auto it = uniformLocationCache.find(name);
        if (it != uniformLocationCache.end()) {
            return it->second;
        }

        const GLint location = glGetUniformLocation(programId, name.c_str());
        uniformLocationCache[name] = location;
        return location;
    }

    GLuint Shader::compileShader(const GLenum type, const char* src) {
        const  GLuint shader = glCreateShader(type);

        glShaderSource(shader, 1, &src, nullptr);
        glCompileShader(shader);

        // Errors validation
        GLint success = 0;
        glGetShaderiv(shader, GL_COMPILE_STATUS, &success);

        if (!success) {
            GLint logLength = 0;
            glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &logLength);

            char log[1024] = {};
            const GLsizei maxLen = (logLength < 1024) ? logLength : 1024;
            glGetShaderInfoLog(shader, maxLen, nullptr, log);

            const char* typeName = (type == GL_VERTEX_SHADER) ? "VERTEX" : "FRAGMENT";
            fprintf(stderr, "[shader] failed to compile %s:\n%s\n", typeName, log);

            glDeleteShader(shader);
            return 0;
        }

        return shader;
    }

    void Shader::setFloat(const std::string &name, float value) const {
        glUniform1f(getUniformLocation(name), value);
    }

    void Shader::setVec3(const std::string &name, float x, float y, float z) const {
        glUniform3f(getUniformLocation(name), x, y, z);
    }

    void Shader::setMat4(const std::string &name, const float *matrixData) const {
        glUniformMatrix4fv(getUniformLocation(name), 1, GL_FALSE, matrixData);
    }

    void Shader::use() const {
        glUseProgram(programId);
    }
} // Apogee
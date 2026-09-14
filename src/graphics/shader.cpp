/**
 * @file shader.cpp
 * @brief Implementation of the Shader class, compilation, and uniform management.
 * @author DreGi0
 * @date September 10th, 2026
 */

#include "shader.h"

#include <cstdio>
#include <fstream>
#include <sstream>
#include <stdexcept>

namespace Apogee {
    Shader::Shader(const std::string& vertPath, const std::string& fragPath) {
        const std::string vertSrc = readFile(vertPath);
        const std::string fragSrc = readFile(fragPath);

        const GLuint vs = compileShader(GL_VERTEX_SHADER, vertSrc.c_str());
        if (vs == 0) {
            throw std::runtime_error("[Shader] vertex stage failed: " + vertPath);
        }

        const GLuint fs = compileShader(GL_FRAGMENT_SHADER, fragSrc.c_str());
        if (fs == 0) {
            glDeleteShader(vs); // Avoid GPU garbage
            throw std::runtime_error("[Shader] fragment stage failed: " + fragPath);
        }

        const GLuint program = glCreateProgram();
        glAttachShader(program, vs);
        glAttachShader(program, fs);
        glLinkProgram(program);

        // Cleanup individual shaders after linking
        glDeleteShader(vs);
        glDeleteShader(fs);

        GLint linked = 0;
        glGetProgramiv(program, GL_LINK_STATUS, &linked);
        if (!linked) {
            const std::string log = getProgramInfoLog(program);
            glDeleteProgram(program);

            throw std::runtime_error("[Shader] link failed:\n" + log);
        }

        m_programId = program;
    }

    Shader::Shader(Shader&& other) noexcept :
    m_programId(other.m_programId),
    m_uniformLocationCache(std::move(other.m_uniformLocationCache)) {
        other.m_programId = 0;
        other.m_uniformLocationCache.clear();
    }

    Shader& Shader::operator=(Shader&& other) noexcept {
        if (this == &other) return *this;

        glDeleteProgram(m_programId);
        m_programId = other.m_programId;
        m_uniformLocationCache = std::move(other.m_uniformLocationCache);

        other.m_programId = 0;
        other.m_uniformLocationCache.clear();
        return *this;
    }

    Shader::~Shader() {
        glDeleteProgram(m_programId);
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

    // ----- Private Helpers -----

    std::string Shader::getShaderInfoLog(const GLuint shader) {
        GLint length = 0;

        // Get from OpenGL the warning message length
        glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &length);
        if (length <= 1) return {};  // empty log

        // Pre-allocate string based on the actual log format length
        std::string log(static_cast<size_t>(length), '\0');
        glGetShaderInfoLog(shader, length, nullptr, log.data());

        // Remove the null terminator that OpenGL adds
        log.pop_back();
        return log;
    }

    std::string Shader::getProgramInfoLog(const GLuint program) {
        // Same shape as getShaderInfoLog but querying program state.
        // Note: kept duplicated on purpose since the OpenGL calls differ
        // (glGetShaderiv vs glGetProgramiv), so abstracting it costs more than repeating it.
        GLint length = 0;
        glGetProgramiv(program, GL_INFO_LOG_LENGTH, &length);
        if (length <= 1) return {}; // empty log x2 :)

        std::string log(static_cast<size_t>(length), '\0');
        glGetProgramInfoLog(program, length, nullptr, log.data());
        log.pop_back();
        return log;
    }

    GLint Shader::getUniformLocation(const std::string &name) const {
        if (const auto it = m_uniformLocationCache.find(name); it != m_uniformLocationCache.end()) {
            return it->second;
        }

        const GLint location = glGetUniformLocation(m_programId, name.c_str());
        m_uniformLocationCache[name] = location;
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
            const char* typeName = (type == GL_VERTEX_SHADER ? "Vertex" : "Fragment");
            fprintf(stderr, "[Shader] failed to compile %s shader:\n%s\n", typeName, getShaderInfoLog(shader).c_str());
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
        glUseProgram(m_programId);
    }
} // namespace Apogee

//
// Created by gio on 9/10/26.
//

#pragma once
#include <string>
#include <unordered_map>

#include "gl_loader.h"


namespace Apogee {
    class Shader {
    private:
        GLuint programId = 0;
        mutable std::unordered_map<std::string, GLint> uniformLocationCache;

        static GLuint compileShader(GLenum type, const char* src);
        static std::string readFile(const std::string& path);

        static std::string getShaderInfoLog(GLuint shader);
        static std::string getProgramInfoLog(GLuint program);


        GLint getUniformLocation(const std::string& name) const;

    public:
        Shader(const std::string& vertPath, const std::string& fragPath);

        Shader(const Shader&) = delete;
        Shader& operator=(const Shader&) = delete;

        Shader(Shader&& other) noexcept;
        Shader& operator=(Shader&& other) noexcept;

        ~Shader();

        void use() const;

        // Setters assume use() was called. OpenGL updates the uniform in the active program, not in 'this' object.
        void setFloat(const std::string& name, float value) const;
        void setVec3(const std::string& name, float x, float y, float z) const;
        void setMat4(const std::string& name, const float* matrixData) const;

        GLuint getId() const { return programId; }
    };
} // Apogee

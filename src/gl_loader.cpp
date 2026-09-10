//
// Created by gio on 9/9/26.
//

#include "gl_loader.h"

#include <GLFW/glfw3.h>
#include <cstdio>

namespace Apogee {
    // TODO: Add a good description to what this file is, does, and purpose and 1-2 sentences Max.

    // Definition             |         Name        |             Value
    PFN_glCreateShader              glCreateShader              = nullptr;
    PFN_glShaderSource              glShaderSource              = nullptr;
    PFN_glCompileShader             glCompileShader             = nullptr;
    PFN_glGetShaderiv               glGetShaderiv               = nullptr;
    PFN_glGetShaderInfoLog          glGetShaderInfoLog          = nullptr;
    PFN_glDeleteShader              glDeleteShader              = nullptr;

    PFN_glCreateProgram             glCreateProgram             = nullptr;
    PFN_glAttachShader              glAttachShader              = nullptr;
    PFN_glLinkProgram               glLinkProgram               = nullptr;
    PFN_glGetProgramiv              glGetProgramiv              = nullptr;
    PFN_glGetProgramInfoLog         glGetProgramInfoLog         = nullptr;
    PFN_glGetUniformLocation        glGetUniformLocation        = nullptr;
    PFN_glUniform1f                 glUniform1f                 = nullptr;
    PFN_glUniform3f                 glUniform3f                 = nullptr;
    PFN_glUniformMatrix4fv          glUniformMatrix4fv          = nullptr;
    PFN_glUseProgram                glUseProgram                = nullptr;
    PFN_glDeleteProgram             glDeleteProgram             = nullptr;

    PFN_glGenVertexArrays           glGenVertexArrays           = nullptr;
    PFN_glBindVertexArray           glBindVertexArray           = nullptr;
    PFN_glDeleteVertexArrays        glDeleteVertexArrays        = nullptr;

    PFN_glGenBuffers                glGenBuffers                = nullptr;
    PFN_glBindBuffer                glBindBuffer                = nullptr;
    PFN_glBufferData                glBufferData                = nullptr;
    PFN_glDeleteBuffers             glDeleteBuffers             = nullptr;

    PFN_glVertexAttribPointer       glVertexAttribPointer       = nullptr;
    PFN_glEnableVertexAttribArray   glEnableVertexAttribArray   = nullptr;

    PFN_glDebugMessageCallback      glDebugMessageCallback      = nullptr;

    // ========== LOADER MACHINERY
    static bool g_anyFailed = false;

    /**
     * @brief Loads a platform-agnostic OpenGL function address.
     *
     * Acts as a wrapper around `glfwGetProcAddress` to resolve function pointers
     * across different platforms (e.g., glXGetProcAddress on Linux, wglGetProcAddress on Windows).
     *
     * @param name The null-terminated ASCII string containing the name of the OpenGL function.
     * @return A generic pointer to the requested function, or `nullptr` if the loading fails.
     *
     * @note If the function cannot be resolved, this function logs an error message
     * to `stderr` and sets the global failure flag `g_anyFailed` to `true`.
     */
    static void* loadOne(const char* name) {
        GLFWglproc proc = glfwGetProcAddress(name);

        if (!proc) {
            fprintf(stderr, "[gl_loader] %s not found\n", name);
            g_anyFailed = true;
            return nullptr;
        }
        return reinterpret_cast<void*>(proc);
    }

    #define LOAD(name) name = reinterpret_cast<PFN_##name>(loadOne(#name))

    /**
     * @brief Loads and initializes all required OpenGL function pointers.
     *
     * Resets the global failure flag `g_anyFailed` and sequentially loads core and
     * utility OpenGL function pointers grouped by functional categories
     * (Shaders, Programs, VAOs, VBOs, Vertex Attributes, and Debugging utilities).
     *
     * @return `true` if all critical function pointers were loaded successfully,
     * or `false` if any loading operation failed.
     *
     * @note This function relies on the `LOAD` macro wrapper. Even though optional
     * extensions like the debug callback might be non-fatal, ensure your macro
     * or loader handles them accordingly if they fail.
     */
    bool loadGLFunctions() {
        g_anyFailed = false;

        // Shaders
        LOAD(glCreateShader);
        LOAD(glShaderSource);
        LOAD(glCompileShader);
        LOAD(glGetShaderiv);
        LOAD(glGetShaderInfoLog);
        LOAD(glDeleteShader);

        // Programs
        LOAD(glCreateProgram);
        LOAD(glAttachShader);
        LOAD(glLinkProgram);
        LOAD(glGetProgramiv);
        LOAD(glGetProgramInfoLog);
        LOAD(glUseProgram);
        LOAD(glDeleteProgram);

        // Uniforms
        LOAD(glGetUniformLocation);
        LOAD(glUniform1f);
        LOAD(glUniform3f);
        LOAD(glUniformMatrix4fv);

        // VAO
        LOAD(glGenVertexArrays);
        LOAD(glBindVertexArray);
        LOAD(glDeleteVertexArrays);

        // VBO
        LOAD(glGenBuffers);
        LOAD(glBindBuffer);
        LOAD(glBufferData);
        LOAD(glDeleteBuffers);

        // Attributes
        LOAD(glVertexAttribPointer);
        LOAD(glEnableVertexAttribArray);

        // Debug (4.3+). If it fails is not fatal.
        LOAD(glDebugMessageCallback);

        return !g_anyFailed;
    }

    #undef LOAD // Clear macro

    // ========== DEBUG
    static const char* severityToString(const GLenum severity)
    {
        switch (severity) {
            case GL_DEBUG_SEVERITY_HIGH:         return "HIGH";
            case GL_DEBUG_SEVERITY_MEDIUM:       return "MEDIUM";
            case GL_DEBUG_SEVERITY_LOW:          return "LOW";
            case GL_DEBUG_SEVERITY_NOTIFICATION: return "info";
            default:                             return "?";
        }
    }

    static void APIENTRY debugCallback(GLenum source, GLenum type, GLuint id, GLenum severity,
                                        GLsizei length, const GLchar* message, const void* userParam) {
        if (severity == GL_DEBUG_SEVERITY_NOTIFICATION) return;

        fprintf(stderr, "[GL %s] %s\n", severityToString(severity), message);
    }

    void enableDebugOutput() {
        if (!glDebugMessageCallback) {
            fprintf(stderr, "[gl_loader] debug output not available\n");
            return;
        }

        glEnable(GL_DEBUG_OUTPUT);

        glEnable(GL_DEBUG_OUTPUT_SYNCHRONOUS); // -> ONLY FOR DEVELOPMENT

        glDebugMessageCallback(debugCallback, nullptr);

        fprintf(stderr, "[gl_loader] debug output enabled\n");
    }
} // Apogee

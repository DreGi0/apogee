//
// Created by gio on 9/12/26.
//

#include "gl_debug.h"

#include <cstdio>
#include <glad/gl.h>


namespace Apogee {
    ///
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

    ///
    static void GLAD_API_PTR debugCallback(GLenum source, GLenum type, GLuint id, GLenum severity,
                                        GLsizei length, const GLchar* message, const void* userParam) {

        fprintf(stderr, "[GL %s] %s\n", severityToString(severity), message);
    }

    ///
    void enableDebugOutput() {
        if (!GLAD_GL_VERSION_4_3) {
            fprintf(stderr, "[gl_debug] KHR_debug not available\n");
            return;
        }

        glEnable(GL_DEBUG_OUTPUT);
        glEnable(GL_DEBUG_OUTPUT_SYNCHRONOUS); // ! -> ONLY FOR DEVELOPMENT
        glDebugMessageCallback(debugCallback, nullptr);

        glDebugMessageControl(GL_DONT_CARE, GL_DONT_CARE, GL_DEBUG_SEVERITY_NOTIFICATION,
                              0, nullptr, GL_FALSE);

        fprintf(stderr, "[gl_debug] debug output enabled\n");
    }
} // Apogee

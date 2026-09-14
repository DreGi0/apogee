/**
 * @file main.cpp
 * @brief Main entry point for the Apogee engine.
 *
 * @details Handles the initialization of the GLFW window, OpenGL context creation,
 * GLAD function loading, and runs the primary application rendering loop.
 *
 * @author DreGi0
 * @date September 8th, 2026
 */

#include <cstdio>
#include <cstdlib>
#include <glad/gl.h>
#include <GLFW/glfw3.h>

#include "core/paths.h"
#include "core/window.h"
#include "graphics/gl_debug.h"
#include "graphics/shader.h"
#include "graphics/mesh.h"

/**
 * @brief Entry point. Creates the window, loads GL, and runs the render loop.
 * @return EXIT_SUCCESS on clean shutdown, EXIT_FAILURE if initialization throws.
 */
int main() {
    try {
        // GLFW window initialization
        const Apogee::Window window(640, 480, "Apogee");

        // Load from pointers to functions through GLAD
        const int version = gladLoadGL(reinterpret_cast<GLADloadfunc>(glfwGetProcAddress));
        if (version == 0) {
            fprintf(stderr, "Failed to initialize GLAD\n");
            return EXIT_FAILURE;
        }

        printf("[glad] loaded OpenGL %d.%d\n", GLAD_VERSION_MAJOR(version), GLAD_VERSION_MINOR(version));
        fflush(stdout);

        printf("OpenGL version : %s\n", reinterpret_cast<const char*>(glGetString(GL_VERSION)));
        printf("Renderer       : %s\n", reinterpret_cast<const char*>(glGetString(GL_RENDERER)));
        printf("Vendor         : %s\n", reinterpret_cast<const char*>(glGetString(GL_VENDOR)));
        printf("GLSL version   : %s\n", reinterpret_cast<const char*>(glGetString(GL_SHADING_LANGUAGE_VERSION)));
        fflush(stdout);

#ifdef APOGEE_DEBUG
        Apogee::enableDebugOutput();
#endif

        // Viewport adjustments
        int fbWidth = 0;
        int fbHeight = 0;
        window.getFramebufferSize(fbWidth, fbHeight);
        glViewport(0, 0, fbWidth, fbHeight);

        // Load and compile shaders
        const Apogee::Shader shaderProgram {
            Apogee::assetPath("shaders/triangle.vert"),
            Apogee::assetPath("shaders/triangle.frag")
        };
        printf("[shader] program created OK (id=%u)\n", shaderProgram.getId());

        // Model geometry
        constexpr float vertices[] = {
            // Position (x, y, z)  |  Color (r, g, b)
            0.5f, 0.5f, 0.0f,       0.0f, 0.0f, 1.0f,   // triangle 1: up-right - blue
            -0.5f, -0.5f, 0.0f,     0.0f, 1.0f, 0.0f,   // triangle 1: low-left - green
            0.5f, -0.5f, 0.0f,      1.0f, 0.0f, 0.0f,   // triangle 1: low-right - red

            0.5f, 0.5f, 0.0f,       0.0f, 0.0f, 1.0f,   // triangle 2: up-right - blue
            -0.5f, 0.5f, 0.0f,      1.0f, 0.0f, 0.0f,   // triangle 2: up-left - red
            -0.5f, -0.5f, 0.0f,     0.0f, 1.0f, 0.0f,   // triangle 2: low-left - green
        };

        const Apogee::Mesh triangleMesh(vertices, 6);

        // Background color used by glClear() on every frame
        glClearColor(0.0f, 0.07f, 0.12f, 1.0f);

        // Main rendering loop
        while (!window.shouldClose()) {
            glClear(GL_COLOR_BUFFER_BIT);

            shaderProgram.use();
            shaderProgram.setVec3("uColor", 1.0f, 1.0f, 1.0f);

            triangleMesh.draw();

            window.swapBuffers();
            Apogee::Window::pollEvents();
        }

        fflush(stdout);
    } catch (const std::exception& e) {
        fprintf(stderr, "%s\n", e.what());
        return EXIT_FAILURE;
    }

    return EXIT_SUCCESS;
}

/**
* @file main.cpp
 * @brief Main entry point for the Apogee engine.
 *
 * @details Handles the initialization of the GLFW window, OpenGL context creation,
 * GLAD function loading, and runs the primary application rendering loop.
 *
 * @author André Iraheta
 * @date September 8th 2026
 */

// ========== IMPORTS ==========
#include <cstdio>
#include <cstdlib>
#include <string>

#include <glad/gl.h>
#include <GLFW/glfw3.h>

#include "graphics/gl_debug.h"
#include "core/paths.h"
#include "graphics/shader.h"

/**
 * @namespace Apogee
 * @brief Core namespace encapsulating engine functionality and callbacks.
 */
namespace Apogee {

    // ----- CALLBACKS -----
    /**
     * @brief GLFW error callback handler.
     *
     * @param error An integer representing the GLFW error code.
     * @param description A null-terminated UTF-8 string describing the error.
     */
    static void errorCallback(int error, const char* description)
    {
        fprintf(stderr, "Error: %s\n", description);
    }

    /**
     * @brief GLFW keyboard input callback.
     *
     * @details Handles global key presses. Currently configured to request
     * window closure when the ESCAPE key is pressed.
     *
     * @param window The window that is supposed to receive the event.
     * @param key The keyboard key that was pressed or released.
     * @param scancode The system specific scancode of the key (un-used).
     * @param action GLFW_PRESS, GLFW_RELEASE or GLFW_REPEAT.
     * @param mods Bit field describing which modifier keys were held down (un-used).
     */
    static void keyCallback(GLFWwindow* window, const int key, int scancode, const int action, int mods)
    {
        if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS) {
            glfwSetWindowShouldClose(window, GLFW_TRUE);
        }
    }

    /**
     * @brief GLFW framebuffer resize callback.
     *
     * @details Ensures the OpenGL viewport matches the new window dimensions
     * upon window resizing by the user or the OS.
     *
     * @param window The window whose framebuffer was resized.
     * @param width The new width, in pixels, of the framebuffer.
     * @param height The new height, in pixels, of the framebuffer.
     */
    static void framebufferSizeCallback(GLFWwindow* window, const int width, const int height) {
        glViewport(0, 0, width, height);
    }
} // namespace Apogee

// ========== CORE ==========
/**
 * @brief Application main entry point.
 *
 * @details Bootstraps the environment by initializing GLFW, configuring an
 * OpenGL 4.6 Core context, and setting up initial vertex data. Enters a
 * blocking while-loop for the render cycle until termination is requested.
 *
 * @return EXIT_SUCCESS upon clean termination, EXIT_FAILURE if initialization fails.
 */
int main() {
    // TODO: Refactor to make it less of a God kind of function - It handles to many things
    glfwSetErrorCallback(Apogee::errorCallback);

    if (!glfwInit()) {
        fprintf(stderr, "Failed to initialize GLFW\n");
        return EXIT_FAILURE;
    }

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

#ifdef APOGEE_DEBUG
    glfwWindowHint(GLFW_OPENGL_DEBUG_CONTEXT, GLFW_TRUE);
#endif

    GLFWwindow* window = glfwCreateWindow(640, 480, "Apogee", nullptr, nullptr);
    if (!window) {
        fprintf(stderr, "Failed to create GLFW window\n");
        glfwTerminate();
        return EXIT_FAILURE;
    }

    glfwMakeContextCurrent(window);
    glfwSwapInterval(1);

    const int version = gladLoadGL(reinterpret_cast<GLADloadfunc>(glfwGetProcAddress));
    if (version == 0) {
        fprintf(stderr, "Failed to initialize GLAD\n");
        glfwDestroyWindow(window);
        glfwTerminate();
        return EXIT_FAILURE;
    }

    printf("[glad] loaded OpenGL %d.%d\n", GLAD_VERSION_MAJOR(version), GLAD_VERSION_MINOR(version));
    fflush(stdout);

    glfwSetKeyCallback(window, Apogee::keyCallback);
    glfwSetFramebufferSizeCallback(window, Apogee::framebufferSizeCallback);

    printf("OpenGL version : %s\n", reinterpret_cast<const char*>(glGetString(GL_VERSION)));
    printf("Renderer       : %s\n", reinterpret_cast<const char*>(glGetString(GL_RENDERER)));
    printf("Vendor         : %s\n", reinterpret_cast<const char*>(glGetString(GL_VENDOR)));
    printf("GLSL version   : %s\n", reinterpret_cast<const char*>(glGetString(GL_SHADING_LANGUAGE_VERSION)));
    fflush(stdout);

#ifdef APOGEE_DEBUG
    Apogee::enableDebugOutput();
#endif

    // # Adjust viewport
    {
        int fbWidth = 0, fbHeight = 0;
        glfwGetFramebufferSize(window, &fbWidth, &fbHeight);
        glViewport(0, 0, fbWidth, fbHeight);
    }


    try {
        // --- COMPILE SHADERS
        const Apogee::Shader shaderProgram {
            Apogee::assetPath("shaders/triangle.vert"),
            Apogee::assetPath("shaders/triangle.frag")
        };
        printf("[shader] program created OK (id=%u)\n", shaderProgram.getId());

        // --- TRIANGLE DATA
        // # just to try something appear on screen
        constexpr float vertices[] = {
            // Position (x, y, z)  |  Color (r, g, b)
            0.5f, 0.5f, 0.0f,       0.0f, 0.0f, 1.0f,   // triangle 1: up-right - red
            -0.5f, -0.5f, 0.0f,     0.0f, 1.0f, 0.0f,   // triangle 1: low-left - green
            0.5f, -0.5f, 0.0f,      1.0f, 0.0f, 0.0f,   // triangle 1: low-right - blue

            0.5f, 0.5f, 0.0f,       0.0f, 0.0f, 1.0f,   // triangle 2: up-right - blue
            -0.5f, 0.5f, 0.0f,      1.0f, 0.0f, 0.0f,   // triangle 2: low-left - red
            -0.5f, -0.5f, 0.0f,     0.0f, 1.0f, 0.0f,   // triangle 2: low-right - green
        };

        // --- CREATE VAO & VBO
        GLuint VAO, VBO;

        // # Reserve IDs
        glGenVertexArrays(1, &VAO);
        glGenBuffers(1, &VBO);

        // # Enable first VAO then VBO
        glBindVertexArray(VAO);
        glBindBuffer(GL_ARRAY_BUFFER, VBO);

        // # Copy from RAM to GPU memory
        glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

        // ___ Attribute 0: Positions
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE,
                                      6 * sizeof(float), reinterpret_cast<void*>(0));

        glEnableVertexAttribArray(0);

        // ___ Attribute 1: Color
        glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE,
                                      6 * sizeof(float), reinterpret_cast<void*>(3 * sizeof(float)));

        glEnableVertexAttribArray(1);

        glBindVertexArray(0);

        // --- PRINCIPAL LOOP
        glClearColor(0.0f, 0.07f, 0.12f, 1.0f);

        while (!glfwWindowShouldClose(window)) {
            glClear(GL_COLOR_BUFFER_BIT);

            shaderProgram.use();
            shaderProgram.setVec3("uColor", 1.0f, 1.0f, 1.0f);
            glBindVertexArray(VAO);

            glDrawArrays(GL_TRIANGLES, 0, 6);

            glfwSwapBuffers(window);
            glfwPollEvents();
        }

        fflush(stdout);

        // --- CLEANUP
        glDeleteVertexArrays(1, &VAO);
        glDeleteBuffers(1, &VBO);

    } catch (const std::exception& e) {
        fprintf(stderr, "%s\n", e.what());
        glfwDestroyWindow(window);
        glfwTerminate();
        return EXIT_FAILURE;
    }

    glfwDestroyWindow(window);
    glfwTerminate();
    return EXIT_SUCCESS;
}
#include <cstdio>
#include <cstdlib>
#include <string>

#include "gl_loader.h"
#include <GLFW/glfw3.h>

#include "paths.h"
#include "shader.h"

namespace Apogee {

    // ========== CALLBACKS

    static void errorCallback(int error, const char* description)
    {
        fprintf(stderr, "Error: %s\n", description);
    }

    static void keyCallback(GLFWwindow* window, const int key, int scancode, const int action, int mods)
    {
        if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS) {
            glfwSetWindowShouldClose(window, GLFW_TRUE);
        }
    }

    static void framebufferSizeCallback(GLFWwindow* window, int width, int height) {
        glViewport(0, 0, width, height);
    }
} // Apogee

// ========== MAIN

int main() {
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

    glfwSetKeyCallback(window, Apogee::keyCallback);
    glfwSetFramebufferSizeCallback(window, Apogee::framebufferSizeCallback);

    glfwMakeContextCurrent(window);
    glfwSwapInterval(1);

    printf("OpenGL version : %s\n", reinterpret_cast<const char*>(glGetString(GL_VERSION)));
    printf("Renderer       : %s\n", reinterpret_cast<const char*>(glGetString(GL_RENDERER)));
    printf("Vendor         : %s\n", reinterpret_cast<const char*>(glGetString(GL_VENDOR)));
    printf("GLSL version   : %s\n", reinterpret_cast<const char*>(glGetString(GL_SHADING_LANGUAGE_VERSION)));
    fflush(stdout);

    if (!Apogee::loadGLFunctions()) {
        fprintf(stderr, "Failed to load OpenGL functions\n");
        glfwDestroyWindow(window);
        glfwTerminate();
        return EXIT_FAILURE;
    }

    printf("[gl_loader] all functions are loaded OK\n");
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
        Apogee::glGenVertexArrays(1, &VAO);
        Apogee::glGenBuffers(1, &VBO);

        // # Enable first VAO then VBO
        Apogee::glBindVertexArray(VAO);
        Apogee::glBindBuffer(GL_ARRAY_BUFFER, VBO);

        // # Copy from RAM to GPU memory
        Apogee::glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

        // ___ Attribute 0: Positions
        Apogee::glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE,
                                      6 * sizeof(float), reinterpret_cast<void*>(0));

        Apogee::glEnableVertexAttribArray(0);

        // ___ Attribute 1: Color
        Apogee::glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE,
                                      6 * sizeof(float), reinterpret_cast<void*>(3 * sizeof(float)));

        Apogee::glEnableVertexAttribArray(1);

        Apogee::glBindVertexArray(0);

        // --- PRINCIPAL LOOP
        glClearColor(0.0f, 0.07f, 0.12f, 1.0f);

        while (!glfwWindowShouldClose(window)) {
            glClear(GL_COLOR_BUFFER_BIT);

            shaderProgram.use();
            shaderProgram.setVec3("uColor", 1.0f, 1.0f, 1.0f);
            Apogee::glBindVertexArray(VAO);

            glDrawArrays(GL_TRIANGLES, 0, 6);

            glfwSwapBuffers(window);
            glfwPollEvents();
        }

        fflush(stdout);

        // --- CLEANUP
        Apogee::glDeleteVertexArrays(1, &VAO);
        Apogee::glDeleteBuffers(1, &VBO);

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
/**
 * @file window.cpp
 * @brief Window class implementation and GLFW callback configuration
 * @author DreGi0
 * @date September 12th, 2026
 */

#include <glad/gl.h>
#include <cstdio>
#include <stdexcept>

#include "window.h"

namespace Apogee {
    // ----- Constructor & Methods -----

    Window::Window(const int width, const int height, const std::string& title) {
        glfwSetErrorCallback(errorCallback);

        if (!glfwInit()) {
            throw std::runtime_error("Failed to initialize GLFW");
        }

        // OpenGL 4.6 core profile configuration
        glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
        glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);
        glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

#ifdef APOGEE_DEBUG
        glfwWindowHint(GLFW_OPENGL_DEBUG_CONTEXT, GLFW_TRUE);
#endif

        GLFWwindow* raw = glfwCreateWindow(width, height, title.c_str(), nullptr, nullptr);
        if (!raw) {
            glfwTerminate();
            throw std::runtime_error("Failed to create GLFW window");
        }

        // unique_ptr takes ownership of the raw handle
        m_handle.reset(raw);

        glfwMakeContextCurrent(m_handle.get());
        glfwSwapInterval(1); // Enable V-Sync by default

        // Register event callbacks with active window
        glfwSetKeyCallback(m_handle.get(), keyCallback);
        glfwSetFramebufferSizeCallback(m_handle.get(), framebufferSizeCallback);
    }

    bool Window::shouldClose() const {
        return glfwWindowShouldClose(m_handle.get());
    }

    void Window::swapBuffers() const {
        glfwSwapBuffers(m_handle.get());
    }

    void Window::pollEvents() {
        glfwPollEvents();
    }

    void Window::getFramebufferSize(int &width, int &height) const {
        glfwGetFramebufferSize(m_handle.get(), &width, &height);
    }

    // ----- Callbacks -----

    void Window::errorCallback(const int /*error*/, const char* description) {
        fprintf(stderr, "Error: %s\n", description);
    }

    void Window::keyCallback(GLFWwindow* window, const int key, int /*scancode*/, const int action, int /*mods*/) {
        // Close window when ESC key is pressed.
        if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS) {
            glfwSetWindowShouldClose(window, GLFW_TRUE);
        }
    }

    void Window::framebufferSizeCallback(GLFWwindow* /*window*/, const int width, const int height) {
        // Updates OpenGL rendering canvas when resized.
        glViewport(0, 0, width, height);
    }

} // namespace Apogee

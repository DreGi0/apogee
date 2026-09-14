/**
 * @file window.cpp
 * @brief TODO.
 *
 * @details TODO.
 *
 * @author DreGi0
 * @date September 12th 2026
 */

// ========== IMPORTS ==========
#include <glad/gl.h>
#include <cstdio>
#include <stdexcept>

#include "window.h"

namespace Apogee {
    // ----- CALLBACKS -----
    void Window::errorCallback(const int error, const char* description) {
        fprintf(stderr, "Error: %s\n", description);
    }

    void Window::keyCallback(GLFWwindow* window, const int key, int scancode, const int action, int mods) {
        if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS) {
            glfwSetWindowShouldClose(window, GLFW_TRUE);
        }
    }

    void Window::framebufferSizeCallback(GLFWwindow* window, const int width, const int height) {
        glViewport(0, 0, width, height);
    }

    Window::Window(const int width, const int height, const std::string& title) {
        glfwSetErrorCallback(errorCallback);

        if (!glfwInit()) {
            throw std::runtime_error("Failed to initialize GLFW");
        }

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

        handle.reset(raw);

        glfwMakeContextCurrent(handle.get());
        glfwSwapInterval(1);

        glfwSetKeyCallback(handle.get(), keyCallback);
        glfwSetFramebufferSizeCallback(handle.get(), framebufferSizeCallback);
    }

    bool Window::shouldClose() const {
        return glfwWindowShouldClose(handle.get());
    }

    void Window::swapBuffers() const {
        glfwSwapBuffers(handle.get());
    }

    void Window::pollEvents() {
        glfwPollEvents();
    }

    void Window::getFramebufferSize(int &width, int &height) const {
        glfwGetFramebufferSize(handle.get(), &width, &height);
    }
} // namespace Apogee

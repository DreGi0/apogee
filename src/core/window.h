/**
 * @file window.h
 * @brief TODO.
 *
 * @details TODO.
 *
 * @author DreGi0
 * @date September 12th 2026
 */

#pragma once

// ========== IMPORTS ==========
#include <memory>
#include <string>

#include "GLFW/glfw3.h"


namespace Apogee {

    struct WindowDeleter {
        void operator()(GLFWwindow* w) const noexcept {
            glfwDestroyWindow(w);
            glfwTerminate();
        }
    };

    class Window {
    private:
        std::unique_ptr<GLFWwindow, WindowDeleter> handle;

        static void errorCallback(int error, const char* description);
        static void keyCallback(GLFWwindow* window, int key, int scancode, int action, int mods);
        static void framebufferSizeCallback(GLFWwindow* window, int width, int height);

    public:
        Window(int width, int height, const std::string& title);

        [[nodiscard]] bool shouldClose() const;
        void swapBuffers() const;

        static void pollEvents();
        void getFramebufferSize(int& width, int& height) const;

        [[nodiscard]] GLFWwindow* getHandle() const { return handle.get(); }
    };
} // namespace Apogee

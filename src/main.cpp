#include <cstdio>
#include <cstdlib>

#include "../gl_loader.h"
#include <GLFW/glfw3.h>

// ========== SHADERS (GLSL)

static auto VERTEX_SHADER_SRC = R"(
#version 460 core

layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aColor;

out vec3 vertexColor;

void main()
{
    gl_Position = vec4(aPos, 1.0);

    vertexColor = aColor;
}
)";

static auto FRAGMENT_SHADER_SRC = R"(
#version 460 core

in vec3 vertexColor;

out vec4 FragColor;

void main()
{
    FragColor = vec4(vertexColor, 1.0);
}
)";

// ========== CALLBACKS

static void error_callback(int error, const char* description)
{
    fprintf(stderr, "Error: %s\n", description);
}

static void key_callback(GLFWwindow* window, const int key, int scancode, const int action, int mods)
{
    if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS) {
        glfwSetWindowShouldClose(window, GLFW_TRUE);
    }
}

static void framebuffer_size_callback(GLFWwindow* window, int width, int height) {
    glViewport(0, 0, width, height);
}

// ========== SHADER UTILITY
static GLuint compileShader(GLenum type, const char* src) {
    const  GLuint shader = glCreateShader(type);

    glShaderSource(shader, 1, &src, nullptr);
    glCompileShader(shader);

    // Errors validation
    GLint success = 0;
    glGetShaderiv(shader, GL_COMPILE_STATUS, &success);

    if (!success) {
        GLint logLength = 0;
        glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &logLength);

        char log[1024] = {};
        const GLsizei maxLen = (logLength < 1024) ? logLength : 1024;
        glGetShaderInfoLog(shader, maxLen, nullptr, log);

        const char* typeName = (type == GL_VERTEX_SHADER) ? "VERTEX" : "FRAGMENT";
        fprintf(stderr, "[shader] failed to compile %s:\n%s\n", typeName, log);

        glDeleteShader(shader);
        return 0;
    }

    return shader;
}

static GLuint createShaderProgram(const char* vsSource, const char* fsSource) {
    const GLuint vs = compileShader(GL_VERTEX_SHADER, vsSource);
    if (vs == 0) return 0;

    const GLuint fs = compileShader(GL_FRAGMENT_SHADER, fsSource);
    if (fs == 0) {
        glDeleteShader(vs); // avoid GPU garbage
        return 0;
    }

    const GLuint program = glCreateProgram();
    glAttachShader(program, vs);
    glAttachShader(program, fs);

    glLinkProgram(program);

    GLint success = 0;
    glGetProgramiv(program, GL_LINK_STATUS, &success);

    if (!success) {
        char log[1024] = {};
        glGetProgramInfoLog(program, 1024, nullptr, log);
        fprintf(stderr, "[shader] failed to link \n%s\n", log);

        glDeleteProgram(program);
        glDeleteShader(vs);
        glDeleteShader(fs);
        return 0;
    }

    glDeleteShader(vs);
    glDeleteShader(fs);

    return program;
}

// ========== MAIN

int main() {
    glfwSetErrorCallback(error_callback);

    if (!glfwInit()) {
        fprintf(stderr, "Failed to initialize GLFW\n");
        return EXIT_FAILURE;
    }

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_OPENGL_DEBUG_CONTEXT, GLFW_TRUE);


    GLFWwindow* window = glfwCreateWindow(640, 480, "Apogee", nullptr, nullptr);
    if (!window) {
        fprintf(stderr, "Failed to create GLFW window\n");
        glfwTerminate();
        return EXIT_FAILURE;
    }

    glfwSetKeyCallback(window, key_callback);
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

    glfwMakeContextCurrent(window);
    glfwSwapInterval(1);

    printf("OpenGL version : %s\n", reinterpret_cast<const char*>(glGetString(GL_VERSION)));
    printf("Renderer       : %s\n", reinterpret_cast<const char*>(glGetString(GL_RENDERER)));
    printf("Vendor         : %s\n", reinterpret_cast<const char*>(glGetString(GL_VENDOR)));
    printf("GLSL version   : %s\n", reinterpret_cast<const char*>(glGetString(GL_SHADING_LANGUAGE_VERSION)));
    fflush(stdout);

    if (!loadGLFunctions()) {
        fprintf(stderr, "Failed to load OpenGL functions\n");
        glfwDestroyWindow(window);
        glfwTerminate();
        return EXIT_FAILURE;
    }

    if (!loadGLFunctions()) {
        fprintf(stderr, "Failed to load OpenGL functions\n");
        glfwDestroyWindow(window);
        glfwTerminate();
        return EXIT_FAILURE;
    }
    printf("[gl_loader] all functions are loaded OK\n");
    fflush(stdout);

    enableDebugOutput();

    // # Adjust viewport
    {
        int fbWidth = 0, fbHeight = 0;
        glfwGetFramebufferSize(window, &fbWidth, &fbHeight);
        glViewport(0, 0, fbWidth, fbHeight);
    }

    // --- COMPILE SHADERS
    const GLuint shaderProgram = createShaderProgram(VERTEX_SHADER_SRC, FRAGMENT_SHADER_SRC);

    if (shaderProgram == 0) {
        fprintf(stderr, "Failed to create shader program\n");
        glfwDestroyWindow(window);
        glfwTerminate();
        return EXIT_FAILURE;
    }

    printf("[shader] program created OK (id=%u)\n", shaderProgram);
    fflush(stdout);

    // --- TRIANGLE DATA
    // # just to try something appear on screen
    const float vertices[] = {
      // Position (x, y, z)  |  Color (r, g, b)
        0.5f, 0.5f, 0.0f,       1.0f, 0.0f, 0.0f,   // up - red
        -0.5f, -0.5f, 0.0f,     0.0f, 1.0f, 0.0f,   // down left - green
        0.5f, -0.5f, 0.0f,     0.0f, 0.0f, 1.0f     // down right - blue
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

        glUseProgram(shaderProgram);
        glBindVertexArray(VAO);

        glDrawArrays(GL_TRIANGLES, 0, 3);

        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    // --- CLEANUP
    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);
    glDeleteProgram(shaderProgram);

    glfwDestroyWindow(window);
    glfwTerminate();
    return EXIT_SUCCESS;
}
//
// Created by gio on 9/9/26.
//

#pragma once
// TODO: Add a good description to what this file is, does, and purpose and 1-2 sentences Max.

// ========== DEPENDENCIES
#include <GL/gl.h>
#include <cstddef>

// ========== TYPES OPENGL 1.1 DOES NOT KNOW
typedef char GLchar;
typedef ptrdiff_t GLsizeiptr;
typedef void (APIENTRY *GLDEBUGPROC)(GLenum source, GLenum type, GLuint id, GLenum severity, GLsizei length,
                                        const GLchar* message, const void* userParam);

// ========== CONSTANTS OPENGL 1.1 DOES NOT KNOW
#ifndef GL_ARRAY_BUFFER
#define GL_ARRAY_BUFFER                 0x8892
#endif
#ifndef GL_ELEMENT_ARRAY_BUFFER
#define GL_ELEMENT_ARRAY_BUFFER         0x8893
#endif
#ifndef GL_STATIC_DRAW
#define GL_STATIC_DRAW                  0x88E4
#endif
#ifndef GL_FRAGMENT_SHADER
#define GL_FRAGMENT_SHADER              0x8B30
#endif
#ifndef GL_VERTEX_SHADER
#define GL_VERTEX_SHADER                0x8B31
#endif
#ifndef GL_COMPILE_STATUS
#define GL_COMPILE_STATUS               0x8B81
#endif
#ifndef GL_LINK_STATUS
#define GL_LINK_STATUS                  0x8B82
#endif
#ifndef GL_INFO_LOG_LENGTH
#define GL_INFO_LOG_LENGTH              0x8B84
#endif

// --- Debug constants (OpenGL 4.3+)
#ifndef GL_DEBUG_OUTPUT
#define GL_DEBUG_OUTPUT                 0x92E0
#endif
#ifndef GL_DEBUG_OUTPUT_SYNCHRONOUS
#define GL_DEBUG_OUTPUT_SYNCHRONOUS     0x8242
#endif
#ifndef GL_DEBUG_SEVERITY_HIGH
#define GL_DEBUG_SEVERITY_HIGH          0x9146
#endif
#ifndef GL_DEBUG_SEVERITY_MEDIUM
#define GL_DEBUG_SEVERITY_MEDIUM        0x9147
#endif
#ifndef GL_DEBUG_SEVERITY_LOW
#define GL_DEBUG_SEVERITY_LOW           0x9148
#endif
#ifndef GL_DEBUG_SEVERITY_NOTIFICATION
#define GL_DEBUG_SEVERITY_NOTIFICATION  0x826B
#endif

// ========== POINTER & FUNCTIONS
// --- Shaders
typedef GLuint (APIENTRY *PFN_glCreateShader)(GLenum type);
extern PFN_glCreateShader glCreateShader;

typedef void (APIENTRY *PFN_glShaderSource)(GLuint shader, GLsizei count,
                const GLchar* const* string, const GLint* length);
extern PFN_glShaderSource glShaderSource;

typedef void (APIENTRY *PFN_glCompileShader)(GLuint shader);
extern PFN_glCompileShader glCompileShader;

typedef void (APIENTRY *PFN_glGetShaderiv)(GLuint shader, GLenum pname, GLint* params);
extern PFN_glGetShaderiv glGetShaderiv;

typedef void (APIENTRY *PFN_glGetShaderInfoLog)(GLuint shader, GLsizei bufSize,
                GLsizei* length, GLchar* infoLog);
extern PFN_glGetShaderInfoLog glGetShaderInfoLog;

typedef void (APIENTRY *PFN_glDeleteShader)(GLuint shader);
extern PFN_glDeleteShader glDeleteShader;

// --- Programs (shaders linked)
typedef GLuint (APIENTRY *PFN_glCreateProgram)();
extern PFN_glCreateProgram glCreateProgram;

typedef void (APIENTRY *PFN_glAttachShader)(GLuint program, GLuint shader);
extern PFN_glAttachShader glAttachShader;

typedef void (APIENTRY *PFN_glLinkProgram)(GLuint program);
extern PFN_glLinkProgram glLinkProgram;

typedef void (APIENTRY *PFN_glGetProgramiv)(GLuint program, GLenum pname, GLint* params);
extern PFN_glGetProgramiv glGetProgramiv;

typedef void (APIENTRY *PFN_glGetProgramInfoLog)(GLuint program, GLsizei bufSize,
                GLsizei* length, GLchar* infoLog);
extern PFN_glGetProgramInfoLog glGetProgramInfoLog;

typedef void (APIENTRY *PFN_glUseProgram)(GLuint program);
extern PFN_glUseProgram glUseProgram;

typedef void (APIENTRY *PFN_glDeleteProgram)(GLuint program);
extern PFN_glDeleteProgram glDeleteProgram;

// --- Vertex Array Objects (VAO)
typedef void (APIENTRY *PFN_glGenVertexArrays)(GLsizei n, GLuint* arrays);
extern PFN_glGenVertexArrays glGenVertexArrays;

typedef void (APIENTRY *PFN_glBindVertexArray)(GLuint array);
extern PFN_glBindVertexArray glBindVertexArray;

typedef void (APIENTRY *PFN_glDeleteVertexArrays)(GLsizei n, const GLuint* arrays);
extern PFN_glDeleteVertexArrays glDeleteVertexArrays;

// --- Vertex Buffer Objects (VBO)
typedef void (APIENTRY *PFN_glGenBuffers)(GLsizei n, GLuint* buffers);
extern PFN_glGenBuffers glGenBuffers;

typedef void (APIENTRY *PFN_glBindBuffer)(GLenum target, GLuint buffer);
extern PFN_glBindBuffer glBindBuffer;

typedef void (APIENTRY *PFN_glBufferData)(GLenum target, GLsizeiptr size, const void* data,
                                            GLenum usage);
extern PFN_glBufferData glBufferData;

typedef void (APIENTRY *PFN_glDeleteBuffers)(GLsizei n, const GLuint* buffers);
extern PFN_glDeleteBuffers glDeleteBuffers;

// --- Vertex attributes
typedef void (APIENTRY *PFN_glVertexAttribPointer)(GLuint index, GLint size,GLenum type,
                                                    GLboolean normalized, GLsizei stride,
                                                    const void* pointer);
extern PFN_glVertexAttribPointer glVertexAttribPointer;

typedef void (APIENTRY *PFN_glEnableVertexAttribArray)(GLuint index);
extern PFN_glEnableVertexAttribArray glEnableVertexAttribArray;

// --- Debug (OpenGL 4.3+)
typedef void (APIENTRY *PFN_glDebugMessageCallback)(GLDEBUGPROC callback, const void* userParam);
extern PFN_glDebugMessageCallback glDebugMessageCallback;

// ========== LOADER PUBLIC API
// Loads all the pointers
bool loadGLFunctions();

// Enable OpenGL 4.3+ debug system
void enableDebugOutput();
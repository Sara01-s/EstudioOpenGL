#pragma once

#include <GL/glew.h>
#include <GLFW/glfw3.h>

// Macros para expandir el "debugger" de OpenGL
#ifdef _DEBUG
    #define ASSERT(x) if (!(x)) __debugbreak();// __debugbreak har� un breakpoint pero solo funciona en Visual C++ (es intr�nsico del compilador)
    #define GLCall(x) GLClearError();\
                      x;\
                      ASSERT(GLLogCall(#x, __FILE__, __LINE__))
#else
    #define GLCall(x) x
#endif


void GLClearError();

_NODISCARD bool GLLogCall(const char* function, const char* file, int line);


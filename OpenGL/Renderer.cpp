#include <iostream >
#include "Renderer.h"

void GLClearError() {
    // tambien se puede escribir como
    // while (!glGetError());
    // ya que está garantizado que glGetError retorne 0 o 1
    // desc: https://docs.gl/gl4/glGetError
    while (glGetError() != GL_NO_ERROR);
}

_NODISCARD bool GLLogCall(const char* function, const char* file, int line) {
    // Mientras que glGetError no retorne 0 (o sea un error)
    while (GLenum error = glGetError()) {
        std::cout << "\n[OpenGL Error] (" << error << "): "
            << function << " " << file << ":" << line << '\n';
        return false;
    }

    return true;
}

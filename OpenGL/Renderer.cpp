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
    bool sucess = true;
    while (GLenum error = glGetError()) {
        sucess = false;
        if (error == 1282) {
            std::cout << "\n[OpenGL Error] (Invalid Operation): "
                << function << " " << file << ":" << line << '\n';
        }
        else {
            std::cout << "\n[OpenGL Error] (" << error << "): "
                << function << " " << file << ":" << line << '\n';
        }
    }

    return sucess;
}

void Renderer::ClearScreen() const {
    GLCall(glClearColor(0.2f, 0.3f, 0.8f, 1.0f));
    GLCall(glClear(GL_COLOR_BUFFER_BIT));
}

void Renderer::Draw(const VertexArray& vertexArray, const IndexBuffer& indexBuffer, const Shader& shader) const {

    shader.Bind();
    vertexArray.Bind();
    indexBuffer.Bind();

    // Draw call para dibujar elementos usando index buffers
    // el último parámetro es el puntero al index buffer
    // pero ya que está bindeado, no es necesario pasarlo.
    // Ahora mismo asumimos que el tipo de data es unsigned int
    // desc: https://docs.gl/gl4/glDrawElements
    GLCall(glDrawElements(GL_TRIANGLES, indexBuffer.GetCount(), GL_UNSIGNED_INT, nullptr));

}

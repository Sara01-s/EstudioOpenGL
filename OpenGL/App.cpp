#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include <iostream> // Para loggear
#include <fstream>  // Para traducir shaders (ifstream)
#include <string>   // Para leer archivos (getline)
#include <sstream>  // Para leer y guardar los vertex y fragment (stringstream) (es como StringBuilder en C#)

#include "Renderer.h"
#include "VertexBuffer.h"
#include "IndexBuffer.h"
#include "VertexArray.h"
#include "Shader.h"

int main(void) {

    GLFWwindow* window;

    /* Initialize the library */
    if (!glfwInit())
        return -1;

    /* Create a windowed mode window and its OpenGL context */
    window = glfwCreateWindow(640, 480, "Mi primera ventana de OpenGL owo", NULL, NULL);
    if (!window) {
        glfwTerminate();
        return -1;
    }

    /* Make the window's context current */
    glfwMakeContextCurrent(window);

    // Esta función sincroniza el frame rate de nuestra aplicacion con el VSync
    glfwSwapInterval(1);

    // Podemos inicializar glew solo en un contexto de renderizado válido de OpenGL
    if (glewInit() != GLEW_OK)
        std::cout << "Failed to initialize GLEW!" << '\n';

    std::cout << "Using OpenGL version " << glGetString(GL_VERSION) << '.\n';

    { // Este scope es para eliminar las allocaciones de memoria en stack
    
        // Ponemos data en nuestro GL_ARRAY_BUFFER con ID bufferID.
        // Para eso tenemos que especificar su tamaño.
        // En este caso queremos guardar 6 vértices de 2 dimensiones (6 vector2) => (3x, 3y)
        // El tamaño de un GL_ARRAY_Buffer está en bytes.
        float dataToGPU[] = {
             -0.5f, -0.5f, // index 0
              0.5f, -0.5f, // index 1
              0.5f,  0.5f, // index 2

             -0.5f,  0.5f, // index 3
        };

        // Recordar que los triángulos se dibujan en sentido antehorario
        unsigned int indices[] = {
            0, 1, 2, // Primer triángulo (que consiste de indices 0, 1, 2) (ver dataToGPU)
            2, 3, 0  // Segundo triángulo (que consiste de indices 2, 3, 0)
        };

        // Vengo del futuro y el core de OpenGL no nos da un Vertex Array Object (VAO) por defecto
        // por lo que crearemos uno antes de crear el buffer
        unsigned int vertexArrayObj;
        // desc: https://docs.gl/gl4/glGenVertexArrays
        GLCall(glGenVertexArrays(1, &vertexArrayObj));
        // luego lo bindeamos
        // desc: https://docs.gl/gl4/glBindVertexArray
        GLCall(glBindVertexArray(vertexArrayObj));

        VertexArray vertexArray;
        VertexBuffer vertexBuffer{ dataToGPU, 4 * 2 * sizeof(float) };
        VertexBufferLayout bufferLayout;

        // float porque el tipo de datos que tenemos en el buffer son floats
        bufferLayout.Push<float>(2);
        vertexArray.AddBuffer(vertexBuffer, bufferLayout);

        GLCall(glEnableVertexAttribArray(0));
        GLCall(glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float), 0));

        IndexBuffer indexBuffer{ indices, 6 * sizeof(unsigned int) };

        // desc: https://docs.gl/gl4/glVertexAttribPointer

        Shader shader { "res/shaders/shader_basic_vertex.glsl", "res/shaders/shader_basic_fragment.glsl" };
        shader.Bind();
        shader.SetUniform4F("u_Color", 1.0f, 1.0f, 1.0f, 1.0f);

        vertexArray.Unbind();
        vertexBuffer.Unbind();
        indexBuffer.Unbind();
        shader.Unbind();

        float red = 0.0f;
        float increment = 0.01f;

        // Mantener un loop hasta que la ventana se cierre
        while (!glfwWindowShouldClose(window)) {
            glClear(GL_COLOR_BUFFER_BIT);

            shader.Bind();
            shader.SetUniform4F("u_Color", red, 1.0f, 1.0f, 1.0f);

            vertexArray.Bind();
            indexBuffer.Bind();
            
            // Draw call para dibujar elementos usando index buffers
            // el último parámetro es el puntero al index buffer
            // pero ya que está bindeado, no es necesario pasarlo.
            // desc: https://docs.gl/gl4/glDrawElements
            GLCall(glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, nullptr));

            // Animar una variable uniforme :)
            if (red > 1.0f) increment = -0.01f;
            else if (red < 0.0f) increment = 0.01f;
            red += increment;

            glfwSwapBuffers(window);
            glfwPollEvents();
        }
    }

    glfwTerminate();
    return 0;
}
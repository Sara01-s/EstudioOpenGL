#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include <iostream> // Para loggear

#include "GLFWController.h"
#include "Renderer.h"
#include "VertexBuffer.h"
#include "IndexBuffer.h"
#include "VertexArray.h"
#include "Shader.h"
#include "Texture.h"

int main(void) {

    GLFWController glfw{};

    if (glfw.Initialize() == -1) {
        return -1;
    }
    
    // El tamaño de un GL_ARRAY_Buffer está en bytes.
    float vertexBufferData[] = {
        -0.5f, -0.5f,   0.0f, 0.0f, // index 0
         0.5f, -0.5f,   1.0f, 0.0f, // index 1
         0.5f,  0.5f,   1.0f, 1.0f, // index 2

        -0.5f,  0.5f,   0.0f, 1.0f  // index 3
    };

    // Recordar que los triángulos se dibujan en sentido antehorario
    unsigned int indexBufferData[6] = {
        0, 1, 2, // Primer triángulo (que consiste de indices 0, 1, 2) (ver dataToGPU)
        2, 3, 0  // Segundo triángulo (que consiste de indices 2, 3, 0)
    };
     
    // Después veremos esta cosilla hehehe
    // desc: https://docs.gl/gl4/glBlendFunc
    //GLCall(glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC1_ALPHA));
    //GLCall(glEnable(GL_BLEND));

    // Vengo del futuro y el core de OpenGL no nos da un Vertex Array Object (VAO) por defecto
    // por lo que crearemos uno antes de crear el buffer
    unsigned int vertexArrayObj;
    // desc: https://docs.gl/gl4/glGenVertexArrays
    GLCall(glGenVertexArrays(1, &vertexArrayObj));
    // luego lo bindeamos
    // desc: https://docs.gl/gl4/glBindVertexArray
    GLCall(glBindVertexArray(vertexArrayObj));

    VertexArray vertexArray{};
    // 4 vertices con 4 floats cada uno, por eso 4 * 4
    VertexBuffer vertexBuffer{ vertexBufferData, 4 * 4 * sizeof(float) };
    VertexBufferLayout bufferLayout{};

    // float porque el tipo de datos que tenemos en el buffer son floats
    // Pusheamos al layout los dos primeros floats (posición)
    bufferLayout.Push<float>(2);
    // Pusheamos al layout los dos segundos floats (texcoords)
    bufferLayout.Push<float>(2);
    vertexArray.AddBuffer(vertexBuffer, bufferLayout);

    IndexBuffer indexBuffer{ indexBufferData, 6 * sizeof(unsigned int) };

    Shader shader { "res/shaders/shader_basic_vertex.glsl", "res/shaders/shader_basic_fragment.glsl" };
    shader.Bind();
    shader.SetUniformFloat4("u_Color", 1.0f, 1.0f, 1.0f, 1.0f);

    Texture texture{ "res/textures/tex_test.png" };
    texture.Bind(); // se asignará al slot 0

    // EL 0, es por el SLOT de TEXTURA 0
    shader.SetUniformInt("u_Texture", 0);


    vertexArray.Unbind();
    vertexBuffer.Unbind();
    indexBuffer.Unbind();
    shader.Unbind();

    float red = 0.0f;
    float increment = 0.01f;

    Renderer renderer{};

    // Mantener un loop hasta que la ventana se cierre
    while (!glfwWindowShouldClose(glfw.GetWindow())) {

        renderer.ClearScreen();

        shader.Bind();
        shader.SetUniformFloat4("u_Color", red, 0.8f, 0.4f, 1.0f);

        renderer.Draw(vertexArray, indexBuffer, shader);
        
        // Animar una variable uniforme :)
        if (red > 1.0f) increment = -0.01f;
        else if (red < 0.0f) increment = 0.01f;
        red += increment;

        glfwSwapBuffers(glfw.GetWindow());
        glfwPollEvents();
    }

    return 0;
}
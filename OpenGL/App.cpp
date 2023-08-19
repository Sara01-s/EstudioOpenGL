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

#include "GLM/glm/glm.hpp"
#include "GLM/glm/gtc/matrix_transform.hpp"

#include "imgui/imgui.h"
#include "imgui/imgui_impl_glfw.h"
#include "imgui/imgui_impl_opengl3.h"

int main(void) {

    GLFWController glfw{};

    if (glfw.Initialize() == -1) {
        return -1;
    }
    
    // El tamaño de un GL_ARRAY_Buffer está en bytes.
    // las 2 columnas de la izquierda son posiciones
    // las 2 columnas de la derecha son texcoords
    float vertexBufferData[] = {
         -50.0f, -50.0f,   0.0f, 0.0f, // index 0
          50.0f, -50.0f,   1.0f, 0.0f, // index 1
          50.0f,  50.0f,   1.0f, 1.0f, // index 2

         -50.0f,  50.0f,   0.0f, 1.0f  // index 3
    };

    // Recordar que los triángulos se dibujan en sentido antehorario
    unsigned int indexBufferData[6] = {
        0, 1, 2, // Primer triángulo (que consiste de indices 0, 1, 2) (ver dataToGPU)
        2, 3, 0  // Segundo triángulo (que consiste de indices 2, 3, 0)
    };
     
    // Después veremos esta cosilla hehehe
    // desc: https://docs.gl/gl4/glBlendFunc
    //GLCall(glEnable(GL_BLEND));
    //GLCall(glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC1_ALPHA));

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

    // 6 es el número de floats que hay en nuestro indexbuffer
    IndexBuffer indexBuffer{ indexBufferData, 6 * sizeof(unsigned int) };

    // Creamos una matriz de proyección (el espacio del mundo)
    // si multiplicamos los primeros 4 parametros, obtienes 4 * 3 (el aspect ratio actual)
    glm::mat4 projection = glm::ortho(0.0f, 960.0f, 0.0f, 540.0f, -1.0f, 1.0f);
    glm::mat4 view = glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, 0.0f, 0.0f)); // mover view (cámara)

    Shader shader{ "res/shaders/shader_basic_vertex.glsl", "res/shaders/shader_basic_fragment.glsl" };
    shader.Bind(); 
    shader.SetUniformFloat4("u_Color", 1.0f, 1.0f, 1.0f, 1.0f);

    Texture texture{ "res/textures/tex_hormiga.png" };
    texture.Bind(); // se asignará al slot 0 por defecto
    shader.SetUniformInt("u_Texture", 0); // EL 0, es por el SLOT de TEXTURA 0

    vertexArray.Unbind();
    vertexBuffer.Unbind();
    indexBuffer.Unbind();
    shader.Unbind();

    float red = 0.0f;
    float increment = 0.01f;

    Renderer renderer{};

    // Creamos el contexto para IMGUI

    ImGui::CreateContext();
    ImGui_ImplGlfw_InitForOpenGL(glfw.GetWindow(), true);
    const char* glslVersion = "#version 330";
    ImGui_ImplOpenGL3_Init(glslVersion);
    ImGui::StyleColorsDark();

    glm::vec3 translation = glm::vec3(200.0f, 200.0f, 0.0f);

    // Mantener un loop hasta que la ventana se cierre
    while (!glfwWindowShouldClose(glfw.GetWindow())) {

        renderer.ClearScreen();

        /* Im Gui */
        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplGlfw_NewFrame();
        ImGui::NewFrame();

        glm::mat4 model = glm::translate(glm::mat4(1.0f), translation);
        glm::mat4 mvp = projection * view * model;

        {
            ImGui::Begin("Ventanita! :)");
            ImGui::SliderFloat3("Translation", &translation.x, 0.0f, 960.0f);
            ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);
            ImGui::End();
        }

        /* Im Gui */

        shader.Bind();
        shader.SetUniformFloat4("u_Color", red, 0.8f, 0.4f, 1.0f);
        shader.SetUniformMat4F("u_ModelViewProjection", mvp);

        renderer.Draw(vertexArray, indexBuffer, shader);
        
        // Animar una variable uniforme :)
        if (red > 1.0f) increment = -0.01f;
        else if (red < 0.0f) increment = 0.01f;
        red += increment;

        ImGui::Render();
        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

        glfwSwapBuffers(glfw.GetWindow());
        glfwPollEvents();
    }

    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();

    return 0;
}
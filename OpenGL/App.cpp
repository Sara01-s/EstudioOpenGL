#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include <iostream> // Para loggear

#include "GLFWController.h"
#include "Renderer.h"

#include "imgui/imgui.h"
#include "imgui/imgui_impl_glfw.h"
#include "imgui/imgui_impl_opengl3.h"

#include "TestClearColor.h"
#include "TestMatrices.h"

int main(void) {

    GLFWController glfw{};

    if (glfw.Initialize() == -1) {
        return -1;
    }

    // Verifica si el contexto de OpenGL es válido
    if (glfwGetCurrentContext() == NULL)
        std::cout << "No hay un contexto de OpenGL valido.\n";
    else
        std::cout << "El contexto de OpenGL es valido.\n";

    
    Renderer renderer{};

    // Creamos el contexto para IMGUI
    ImGui::CreateContext();
    ImGui_ImplGlfw_InitForOpenGL(glfw.GetWindow(), true);
    const char* glslVersion = "#version 330 core";
    ImGui_ImplOpenGL3_Init(glslVersion);
    ImGui::StyleColorsDark();

    // Tests::TestClearColor test{};
    Tests::TestMatrices test {
          "res/shaders/shader_basic_vertex.glsl"
        , "res/shaders/shader_basic_fragment.glsl"
        , "res/textures/logo_vamvam.png"
    };


    //GLCall(glEnable(GL_BLEND));
    //GLCall(glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA));

    // Mantener un loop hasta que la ventana se cierre
    while (!glfwWindowShouldClose(glfw.GetWindow())) {

        renderer.ClearScreen();

        test.OnUpdate(0.0f);
        test.OnRender();

        /* Im Gui */
        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplGlfw_NewFrame();
        ImGui::NewFrame();

        test.OnGuiRender();

        ImGui::Render();
        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
        /* Im Gui */

        glfwSwapBuffers(glfw.GetWindow());
        glfwPollEvents();
    }

    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();

    return 0;
}
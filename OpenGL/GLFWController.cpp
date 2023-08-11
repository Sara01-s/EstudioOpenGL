#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <iostream>

#include "GLFWController.h"

GLFWController::~GLFWController() {
    glfwTerminate();
}

int GLFWController::Initialize() {

    // Inicializar GLFW
    if (!glfwInit()) {
        std::cout << "No se pudo inicializar GLFW\n";
        return -1;
    }

    m_Window = glfwCreateWindow(640, 480, "Ventana de OpenGL >:)", NULL, NULL);

    // Crear ventana flotante
    if (!m_Window) {
        std::cout << "No se pudo crear la ventana de GLFW\n";
        glfwTerminate();
        return -1;
    }

    // Asignar la ventana en uso como contexto de OpenGL
    glfwMakeContextCurrent(m_Window);

    // Esta función sincroniza el frame rate de nuestra aplicacion con el VSync
    glfwSwapInterval(1);

    // Podemos inicializar glew solo en un contexto de renderizado válido de OpenGL
    if (glewInit() != GLEW_OK) {
        std::cout << "No se pudo inicializar GLEW\n";
        return -1;
    }

    std::cout << "Utilizando version de OpenGL: " << glGetString(GL_VERSION) << '.\n';

}

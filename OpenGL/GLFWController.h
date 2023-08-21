#pragma once
#include <GL/glew.h>
#include <GLFW/glfw3.h>

class GLFWController {

public:

	GLFWController() = default;
	~GLFWController();

	int Initialize();

	inline GLFWwindow* GetWindow() const { return m_Window; }

private:
	GLFWwindow* m_Window = nullptr;
};


#pragma once

class GLFWController {

public:

	GLFWController() = default;
	~GLFWController();

	int Initialize();

	inline GLFWwindow* GetWindow() const { return m_Window; }

private:
	GLFWwindow* m_Window = nullptr;
};


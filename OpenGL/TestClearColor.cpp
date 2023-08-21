#include "TestClearColor.h"
#include "Renderer.h"
#include "imgui/imgui.h"

namespace Tests {

	TestClearColor::TestClearColor()
		: m_ClearColor { 0.2f, 0.3f, 0.8f, 1.0f }
	{
	}

	TestClearColor::~TestClearColor()
	{
	}

	void TestClearColor::OnUpdate(const float deltaTime)
	{
	}

	void TestClearColor::OnRender() {
		// Establece el color default de clear
		GLCall(glClearColor(m_ClearColor[0], m_ClearColor[1], m_ClearColor[2], m_ClearColor[3]));
		// Reestablece el buffer de colores al default
		GLCall(glClear(GL_COLOR_BUFFER_BIT));
	}

	void TestClearColor::OnGuiRender() {
		ImGui::ColorEdit4("Clear color", m_ClearColor);
	}

} // namespace Tests
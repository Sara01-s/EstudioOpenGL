#pragma once
#include "Test.h"
#include "Shader.h"
#include "Renderer.h"
#include "VertexBuffer.h"
#include "IndexBuffer.h"
#include "Texture.h"

namespace Tests {

	class TestMatrices : public Test {

	public:
		TestMatrices(const std::string& vertShader, const std::string& fragShader, const std::string& texPath);
		~TestMatrices();

		void OnUpdate(const float deltaTime) final override;
		void OnRender() final override;
		void OnGuiRender() final override;
		
	private:
		Renderer m_Renderer{};

		VertexArray m_VertexArray{};
		IndexBuffer m_IndexBuffer;
		Texture m_Texture;
		Shader m_Shader;

	};
} // namespace Tests


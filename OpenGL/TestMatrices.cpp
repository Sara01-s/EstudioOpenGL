#include "TestMatrices.h"
#include "VertexArray.h"
#include "VertexBufferLayout.h"

#include "GLM/glm/glm.hpp"
#include "GLM/glm/gtc/matrix_transform.hpp"
#include "GLM/glm/matrix.hpp"
#include "Utils.h"

namespace Tests {

	TestMatrices::TestMatrices(const std::string& vertShader, const std::string& fragShader, const std::string& texPath)
		: m_Shader { vertShader, fragShader }, m_Texture { texPath }
	{

		//float vertexData[] = {
		////		    Positions	      //**/   TexCoords   //
		//	-1.0f, -1.0f, 0.0f, 0.0f, //**/  0.0f,  0.0f, // Index 0
		//	 1.0f, -1.0f, 0.0f, 0.0f, //**/  1.0f, -1.0f, // Index 1
		//	 1.0f,  1.0f, 0.0f, 0.0f, //**/  1.0f,  1.0f, // Index 2

		//	-1.0f,  1.0f, 0.0f, 0.0f, //**/ -1.0f,  1.0f  // Index 3
		//};

		float vertexData[] = {
		 100.0f, 100.0f,    // index 0
		 200.0f, 100.0f,    // index 1
		 200.0f, 200.0f,    // index 2

		 100.0f, 200.0f  // index 3
		};

		unsigned int indexData[] = {
			0, 1, 2, // TRIANGLE 1
			2, 3, 0  // TRIANGLE 2
		};

		// 6 columnas * 4 filas en vertexData
		VertexBuffer vb{ vertexData, 4 * 2 * sizeof(float) };
		VertexBufferLayout vbl{};

		// float porque queremos pushear floats jjdsjds
		// 3 porque es el número de posiciones por fila de vertexData
		vbl.Push<float>(2);
		// 2 porque es el número de texCoords por fila de vertexData
		//vbl.Push<float>(2);
		// vb dará los bytes de datos, vbl dirá como interpretar esa data
		m_VertexArray.AddBuffer(vb, vbl);
		// 6 es la cantidad de uints en indexData
		m_IndexBuffer = { indexData, 6 * sizeof(unsigned int) };
		//m_Texture.Bind(0);

		//glm::mat4 projection = glm::ortho(-5.0f, 5.0f, -5.0f, 5.0f, -1.0f, 1.0f);
		glm::mat4 projection = glm::ortho(0.0f, 960.0f, 0.0f, 540.0f, -1.0f, 1.0f);

		//glm::mat4 projection = glm::perspective (
		//	  glm::radians(85.0f)	// Field of view FOV
		//	, 16.0f / 9.0f			// Aspect ratio
		//	, 0.1f					// Near clip plane
		//	, 100.0f				// Far clip plane
		//);

		//glm::mat4 view = glm::lookAt (
		//	  glm::vec3(4, 4, -3)	// coordenadas world space
		//	, glm::vec3(0, 0, 0)	// mira hacia el origen (en world space)
		//	, glm::vec3(0, 1, 0)	// el "arriba" de la cámara (poner -1 en Y la daría vuelta)
		//);
		glm::mat4 view = glm::mat4(1.0f);
		glm::mat4 model = glm::mat4(1.0f); // identity matrix, el objeto estará en 0

		glm::mat4 mvp = projection * view * model;

		glm::vec3 position1 = mvp * glm::vec4(100.0f, 100.0f, 0.0f, 0.0f);
		glm::vec3 position2 = mvp * glm::vec4(200.0f, 100.0f, 0.0f, 0.0f);
		glm::vec3 position3 = mvp * glm::vec4(200.0f, 200.0f, 0.0f, 0.0f);
		glm::vec3 position4 = mvp * glm::vec4(200.0f, 100.0f, 0.0f, 0.0f);
		std::cout << "\nVertices del objeto dibujado: \n";
		Utils::PrintVec3(position1);
		Utils::PrintVec3(position2);
		Utils::PrintVec3(position3);
		Utils::PrintVec3(position4);

		m_Shader.Bind();
		/*m_Shader.SetUniformMat4F("u_Model", model);
		m_Shader.SetUniformMat4F("u_View", view);
		m_Shader.SetUniformMat4F("u_Projection", projection);*/
		m_Shader.SetUniformMat4F("u_MVP", mvp);
		//m_Shader.SetUniformInt("u_Texture", 0); // EL 0, es por el SLOT de TEXTURA 0
	}

	TestMatrices::~TestMatrices()
	{
	}

	void TestMatrices::OnUpdate(const float deltaTime)
	{
	}

	void TestMatrices::OnRender() {
		m_Renderer.Draw(m_VertexArray, m_IndexBuffer, m_Shader);
	}

	void TestMatrices::OnGuiRender()
	{
	}

} // namespace Tests


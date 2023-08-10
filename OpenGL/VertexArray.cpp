#include "VertexArray.h"
#include "Renderer.h"

VertexArray::VertexArray() {
	GLCall(glGenVertexArrays(1, &m_RendererID));
}

VertexArray::~VertexArray() {
	GLCall(glDeleteVertexArrays(1, &m_RendererID));
}

void VertexArray::AddBuffer(const VertexBuffer& vb, const VertexBufferLayout& layout) {
	Bind();    // Bindear ESTE array de vértices (Vertex Array)
	vb.Bind(); // Primero bindeamos el buffer usando el objeto VertexBuffer que creamos

	const auto& vertexElements = layout.GetElements();

	unsigned int offset = 0;

	for (unsigned int i = 0; i < vertexElements.size(); i++) {
		const auto& element = vertexElements[i];

		GLCall(glEnableVertexAttribArray(i));
	
		glVertexAttribPointer(i, element.Count, element.Type, 
			element.Normalized, layout.GetStride(), (const void*) offset);

		offset += element.Count * VertexBufferElement::GetSizeOfType(element.Type);
	}

}

void VertexArray::Bind() const {
	GLCall(glBindVertexArray(m_RendererID));
}

void VertexArray::Unbind() const {
	GLCall(glBindVertexArray(0));
}

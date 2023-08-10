#pragma once

#include <vector>
#include <GL/glew.h>
#include <assert.h>

struct VertexBufferElement {
	unsigned int Type;
	unsigned int Count;
	unsigned char Normalized;

	static inline unsigned int GetSizeOfType(unsigned int type) {
		switch (type) {
			case GL_FLOAT		  : return sizeof(GLfloat);
			case GL_UNSIGNED_INT  : return sizeof(GLuint);
			case GL_UNSIGNED_BYTE : return sizeof(GLbyte);
		}

		assert(false);
		return 0;
	}
};

class VertexBufferLayout {

public:
	
	VertexBufferLayout()
		: m_Stride(0) {}

	template<typename T>
	void Push(unsigned int count) {
		assert(false);
	}

	// Queremos que las implementaciones de Push sean especifícas para
	// cada tipo, por eso lanzamos un assert(false) en el genérico
	// NOTA: esto no funciona en g++ o clang, es mejor hacerlo con function overload

	// Incrementamos el stride (puntero que verá que tan grandes son cada uno
	// de los elementos que haya en el vertex buffer (un montón de data))
	// ASI, este puntero sabrá cuando "avanzar" por el buffer para ir obteniendo
	// los distintos tipos de data (col, texcoord, tan, pos).
	template<>
	void Push<float>(unsigned int count) {
		m_Elements.push_back({ GL_FLOAT, count, GL_FALSE });
		m_Stride += count * VertexBufferElement::GetSizeOfType(GL_FLOAT);
	}

	template<>
	void Push<unsigned int>(unsigned int count) {
		m_Elements.push_back({ GL_UNSIGNED_INT, count, GL_FALSE });
		m_Stride += count * VertexBufferElement::GetSizeOfType(GL_UNSIGNED_INT);
	}

	template<>
	void Push<unsigned char>(unsigned int count) {
		m_Elements.push_back({ GL_UNSIGNED_BYTE, count, GL_TRUE });
		m_Stride += count * VertexBufferElement::GetSizeOfType(GL_UNSIGNED_BYTE);
	}

	inline const std::vector<VertexBufferElement> GetElements() const { return m_Elements; }
	inline unsigned int GetStride() const { return m_Stride; }

private:
	// Los elementos de un vertex buffers pueden ser
	// colores, tangentes, texcoords, posiciones, etc...
	// ya que los vertex solo contienen un montón de bytes
	std::vector<VertexBufferElement> m_Elements{};
	unsigned int m_Stride{};
};
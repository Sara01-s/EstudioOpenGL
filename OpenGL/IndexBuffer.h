#pragma once

class IndexBuffer {

public:

private: 
	// Todos los "objetos" de OpenGL (Texturas, Vertex buffers, Shader, Vertex Arrays, etc...)
	// esta ID los identifica
private:
	unsigned int m_RendererID;
	unsigned int m_Count; // Cuántos indices vamos a usar

public:
	IndexBuffer(const unsigned int* data, unsigned int count);
	~IndexBuffer();

	void Bind() const;
	void Unbind() const;

	inline unsigned int GetCount() const { return m_Count; }
};


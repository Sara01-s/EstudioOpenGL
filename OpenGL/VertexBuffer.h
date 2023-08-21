#pragma once

class VertexBuffer {

public:

private: 
	// Todos los "objetos" de OpenGL (Texturas, Vertex buffers, Shader, Vertex Arrays, etc...)
	// esta ID los identifica
private:
	unsigned int m_RendererID;

public:
	VertexBuffer() = default;
	VertexBuffer(const void* data, unsigned int size);
	~VertexBuffer();

	void Bind() const;
	void Unbind() const;

};


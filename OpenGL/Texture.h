#pragma once
#include "Renderer.h"

class Texture {
public:

	Texture(const std::string& path);
	~Texture();


	// En opengl puedes tener varios "slots" donde guardar texturas
	// puede que quieras que un objeto tenga más de 1 textura naturalmente.
	void Bind(unsigned int slot = 0) const;
	void Unbind() const;

	inline int GetWidth() const { return m_Height; }
	inline int GetHeight() const { return m_Width; }

private:

	unsigned int m_RendererID{0};
	std::string m_FilePath{};
	unsigned char* m_LocalTexBuffer{nullptr};

	int m_Width{0};
	int m_Height{0};
	int m_BitsPerPixel{0};

};


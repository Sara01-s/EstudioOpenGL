#include "Texture.h"
#include "stb_image/stb_image.h"

Texture::Texture(const std::string& path)
	: m_FilePath(path)
{
	// Flipeamos la textura para que esté boca abajo
	// esto es porque OpenGL espera que nuestra texturas empiecen
	// desde la esquina inferior izquierda, no desde la esquina superior izquierda
	// nota: 1 es true
	stbi_set_flip_vertically_on_load(1);

	// esta función cambiará interamente los valores de width, height y bpp. parecido a "out" en c#
	// 4 son los canales de colores que deseamos en nuestro caso RGBA (4)
	m_LocalTexBuffer = stbi_load(path.c_str(), &m_Width, &m_Height, &m_BitsPerPixel, 4);

	// Generamos 1 textura y le asignamos un ID
	// La textura no tiene dimensiones, se asume que se especificará al bindearse
	// desc: https://docs.gl/gl4/glGenTextures
	GLCall(glGenTextures(1, &m_RendererID));

	// Bindeamos la textura como "Textura 2D" usando nuestra ID
	// Ahora que está bindeada, procedemos configurarla
	// desc: https://docs.gl/gl4/glBindTexture
	Bind();

	GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR));
	GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR));
	// GL_TEXTURE_WRAP_S es en horizontal
	// GL_TEXTURE_WRAP_T es en vertical
	// GL_CLAMP_TO_EDGE, es hacer un clamp de la textura (cortarla cuando se agrande)
	// la otra opción es Tiled
	GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE));
	GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE));

	// (param 2) es level, por si usas LOD, en este caso no así que 0
	// DESC: https://docs.gl/gl4/glTexImage2D LEE ESTO, ESTÁ INTERESANTE
	GLCall(glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, m_Width, m_Height, 
						0, GL_RGBA, GL_UNSIGNED_BYTE, m_LocalTexBuffer));

	// La unbindeamos pues ya está lista ;)
	Unbind();

	// eliminamos la memoria heap alocada por stbi (basicamente un free())
	if (m_LocalTexBuffer)
		stbi_image_free(m_LocalTexBuffer);
}

Texture::~Texture() {
	// Eliminamos la textura con nuestra ID
	// desc: https://docs.gl/gl4/glDeleteTextures
	GLCall(glDeleteTextures(1, &m_RendererID));
}

void Texture::Bind(unsigned int slot) const {

	// Esto es como un "Hey, quiero que la textura activa sea la del slot X"
	// GL_TEXTURE0 es el primer slot (hay hasta 31)
	// SIN EMBARGO, puede que tu plataforma no soporte 31, esto es solo de OpenGL
	// desc: https://docs.gl/gl4/glActiveTexture
	GLCall(glActiveTexture(GL_TEXTURE0 + slot));
	GLCall(glBindTexture(GL_TEXTURE_2D, m_RendererID));
}

void Texture::Unbind() const {
	GLCall(glBindTexture(GL_TEXTURE_2D, 0));
}

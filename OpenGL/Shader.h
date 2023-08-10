#pragma once

#include <iostream>

struct ShaderSource {
	std::string VertexSource;
	std::string FragmentSource;
};

class Shader {

public:
	Shader(const std::string& vertex, const std::string& fragment);
	~Shader();

	void Bind() const;
	void Unbind() const;

	// usar uniforms
	void SetUniform4F(const std::string& propertyName, float v0, float v1, float f1, float f2);

private:

	unsigned int m_RendererID {};
	std::string m_VertexFilePath;
	std::string m_FragmentFilePath;
	
	unsigned int CompileShader(unsigned int type, const std::string& source);
	unsigned int CreateShader(const std::string& vertexShader, const std::string& fragmentShader);
	int GetUniformLocation(const std::string& propertyName);


};
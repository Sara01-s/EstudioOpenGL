#pragma once

#include <iostream>
#include <unordered_map>

#include <GLM/glm/glm.hpp>

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
	void SetUniformInt(const std::string& propertyName, int value);
	void SetUniformFloat(const std::string& propertyName, float value);
	void SetUniformFloat4(const std::string& propertyName, float v0, float v1, float f1, float f2);
	void SetUniformMat4F(const std::string& propertyName, const glm::mat4& matrix);

private:

	unsigned int m_RendererID{};
	std::string m_VertexFilePath{};
	std::string m_FragmentFilePath{};
	std::unordered_map<std::string, int> m_UniformLocationCache{};
	
	unsigned int CompileShader(unsigned int type, const std::string& source);
	unsigned int CreateShader(const std::string& vertexShader, const std::string& fragmentShader);
	int GetUniformLocation(const std::string& propertyName);


};
#pragma once

#include <string>
#include <unordered_map>
#include <glm/glm.hpp>


struct ShaderProgramSource
{
	std::string VertexSource;
	std::string FragmentSource;
};

class Shader
{
private:
	unsigned int m_RendererID;
	std::string m_FilePath;
	std::unordered_map<std::string, int> m_Uniform_Location_Cache;

public:
	Shader(const std::string& filepath);
	~Shader();

	void Bind() const;
	void UnBind() const;

	//set uniforms
	//matrix
	void SetUniforMat4f(const std::string name, const glm::mat4& matrix);

	//int
	void SetUniform1i(const std::string name, int v0);

	//float
	void SetUniform1f(const std::string name, float v0);
	void SetUniform2f(const std::string name, float v0, float v1);
	void SetUniform3f(const std::string name, float v0, float v1, float v2);
	void SetUniform4f(const std::string name, float v0, float v1, float v2, float v3);

private:
	unsigned int GetUniformLocation(const std::string name);

	ShaderProgramSource ParseShader(const std::string& filepath);
	unsigned int CompileShader(unsigned int type, const std::string& source);
	unsigned int CreateShader(const std::string& VertexShader, const std::string& FragmentShader);
};


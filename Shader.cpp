#include <GL/glew.h>
#include <fstream>
#include <iostream>
#include <sstream>

#include "Shader.h"
#include "Assert.h"


Shader::Shader(const std::string& filepath)
	: m_FilePath(filepath), m_RendererID(0)
{
    ShaderProgramSource source = ParseShader(filepath);
    m_RendererID = CreateShader(source.VertexSource, source.FragmentSource); 
}

Shader::~Shader()
{
    GLCall(glDeleteProgram(m_RendererID));
}

ShaderProgramSource Shader::ParseShader(const std::string& filepath)
{
    enum class ShaderType
    {
        NONE = -1, VERTEX = 0, FRAGMENT = 1
    };
    std::stringstream ss[2];
    ShaderType type = ShaderType::NONE;
 
    std::ifstream file(filepath);
    std::string input;
    while ( std::getline(file, input) )
    {
        if (input.find("#shader") != std::string::npos)
        {
            if (input.find("vertex") != std::string::npos)
            {
                //set mode to vertex
                type = ShaderType::VERTEX;
            }
            else if (input.find("fragment") != std::string::npos)
            {
                //set mode to fragment
                type = ShaderType::FRAGMENT;
            }
        }
        else
        {
            int group = (int)type;
            ss[group] << input << std::endl;
        }
    }
    return { ss[0].str(), ss[1].str() };
}

unsigned int Shader::CompileShader(unsigned int type, const std::string& source)
{
    unsigned int id = glCreateShader(type);
    const char* src = source.c_str();
    GLCall( glShaderSource(id, 1, &src, nullptr) );
    GLCall( glCompileShader(id) );
    return id;
}

unsigned int Shader::CreateShader(const std::string& VertexShader, const std::string& FragmentShader)
{
	unsigned int vertexShader = CompileShader(GL_VERTEX_SHADER, VertexShader);
	unsigned int fragmentShader = CompileShader(GL_FRAGMENT_SHADER, FragmentShader);

	unsigned int shaderProgram;
	shaderProgram = glCreateProgram();

	GLCall(glAttachShader(shaderProgram, vertexShader));
	GLCall(glAttachShader(shaderProgram, fragmentShader));
	GLCall(glLinkProgram(shaderProgram));
	GLCall(glValidateProgram(shaderProgram));

	GLCall(glDeleteShader(vertexShader));
	GLCall(glDeleteShader(fragmentShader));

	return shaderProgram;
}

void Shader::Bind() const
{
    GLCall(glUseProgram(m_RendererID));
}

void Shader::UnBind() const
{
    GLCall(glUseProgram(0));
}

void Shader::SetUniforMat4f(const std::string name, const glm::mat4& matrix)
{
    GLCall(glUniformMatrix4fv(GetUniformLocation(name), 1, GL_FALSE, &matrix[0][0]));
}

void Shader::SetUniform1i(const std::string name, int v0)
{
    GLCall(glUniform1i(GetUniformLocation(name), v0));
}

void Shader::SetUniform1f(const std::string name, float v0)
{
	GLCall(glUniform1f(GetUniformLocation(name), v0));
}

void Shader::SetUniform2f(const std::string name, float v0, float v1)
{
	GLCall(glUniform2f(GetUniformLocation(name), v0, v1));
}

void Shader::SetUniform3f(const std::string name, float v0, float v1, float v2)
{
	GLCall(glUniform3f(GetUniformLocation(name), v0, v1, v2));
}

void Shader::SetUniform4f(const std::string name, float v0, float v1, float v2, float v3)
{
    GLCall(glUniform4f( GetUniformLocation(name), v0, v1, v2, v3));
}

unsigned int Shader::GetUniformLocation(const std::string name)
{
    if (m_Uniform_Location_Cache.find(name) != m_Uniform_Location_Cache.end())
        return m_Uniform_Location_Cache[name];

    GLCall(int location = glGetUniformLocation(m_RendererID, name.c_str()) );

    if (location == -1)
    {
        std::cout << "[Warning]: Uniform: " << name.c_str() << " is not being used" << std::endl;
    }

    m_Uniform_Location_Cache[name] = location;
    return location;
}



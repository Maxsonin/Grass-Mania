#include "ShaderProgram.h"

#include <iostream>
#include <fstream>
#include <sstream>

std::string ShaderProgram::LoadFileAsString(const std::string& filePath)
{
	std::ifstream fileStream(filePath);
	if (!fileStream.is_open())
	{
		std::cerr << "Unable to FIND/OPEN file: " + filePath << std::endl;
		exit(EXIT_FAILURE);
	}

	std::stringstream stringBuffer;
	stringBuffer << fileStream.rdbuf();
	return stringBuffer.str();
}

GLuint ShaderProgram::CompileShader(GLenum type, const std::string& source)
{
	GLuint shaderRendererID = glCreateShader(type);
	const char* shaderSrc = source.c_str();
	glShaderSource(shaderRendererID, 1, &shaderSrc, nullptr);
	glCompileShader(shaderRendererID);

	GLint compileStatus;
	glGetShaderiv(shaderRendererID, GL_COMPILE_STATUS, &compileStatus);
	if (compileStatus == GL_FALSE)
	{
		GLint logLength;
		glGetShaderiv(shaderRendererID, GL_INFO_LOG_LENGTH, &logLength);
		std::string log(logLength, ' '); // or use malloc()
		glGetShaderInfoLog(shaderRendererID, logLength, nullptr, &log[0]);

		std::cerr << "ERROR: Failed to COMPILE Shader: Shader Program - " + m_ShaderProgramName +
			", Shader Type - " + (type == GL_VERTEX_SHADER ? "GL_VERTEX_SHADER" : "GL_FRAGMENT_SHADER")
			+ "\nLog:\n" + log << std::endl;
		exit(EXIT_FAILURE);
	}

	return shaderRendererID;
}

ShaderProgram::ShaderProgram(const std::string& shaderName, const std::string& vertexShaderFilePath, const std::string& fragmentShaderFilePath)
	: m_ShaderProgramName(shaderName)
{
	m_RendererID = glCreateProgram();
	if (m_RendererID == 0)
	{
		std::cerr << "ERROR: Unable to create Shader Program: Name - " + shaderName + ", ID - " + std::to_string(m_RendererID) << std::endl;
		exit(EXIT_FAILURE);
	}

	std::string vertexShaderSource = LoadFileAsString(vertexShaderFilePath);
	std::string fragmentShaderSource = LoadFileAsString(fragmentShaderFilePath);

	GLuint vertexShader   = CompileShader(GL_VERTEX_SHADER, vertexShaderSource);
	GLuint fragmentShader = CompileShader(GL_FRAGMENT_SHADER, fragmentShaderSource);

	glAttachShader(m_RendererID, vertexShader);
	glAttachShader(m_RendererID, fragmentShader);

	glLinkProgram(m_RendererID);
	GLint linkStatus;
	glGetProgramiv(m_RendererID, GL_LINK_STATUS, &linkStatus);
	if (linkStatus == GL_FALSE)
	{
		GLint logLength;
		glGetProgramiv(m_RendererID, GL_INFO_LOG_LENGTH, &logLength);
		std::string log(logLength, ' '); // or use malloc()
		glGetProgramInfoLog(m_RendererID, logLength, nullptr, &log[0]);

		std::cerr << "ERROR: Failed to LINK Shader Program: Name - " + shaderName + "\nLog:\n" + log << std::endl;
		exit(EXIT_FAILURE);
	}

	glDeleteShader(vertexShader);
	glDeleteShader(fragmentShader);
}
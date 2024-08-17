#ifndef SHADER
#define SHADER

#include <GL/gl3w.h>

#include <string>

class ShaderProgram
{
private:
	GLuint m_RendererID = 0;
	std::string m_ShaderProgramName;

	std::string LoadFileAsString(const std::string& filePath);
	GLuint CompileShader(GLenum type, const std::string& source);

public:
	ShaderProgram(std::string& shaderName, const std::string& vertexShader, const std::string& fragmentShader);

	void Bind() const { glUseProgram(m_RendererID); }
	void Unbind() const { glUseProgram(0); }
};

#endif

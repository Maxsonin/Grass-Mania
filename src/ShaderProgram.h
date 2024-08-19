#ifndef SHADER
#define SHADER

#include <GL/gl3w.h>
#include <glm/glm.hpp>

#include <string>
#include <unordered_map>
#include <iostream>

class ShaderProgram
{
private:
	std::string LoadFileAsString(const std::string& filePath);
	GLuint CompileShader(GLenum type, const std::string& source);

    GLint GetUniformLocation(const std::string& name)
    {
        if (m_UniformLocationCache.find(name) != m_UniformLocationCache.end())
            return m_UniformLocationCache[name];

        GLint location = glGetUniformLocation(m_RendererID, name.c_str());
        if (location == -1)
        {
            std::cerr << "ERROR: Uniform " + name + " couldn't be created in " + m_ShaderProgramName + " Shader Program\n";
            exit(EXIT_FAILURE);
        }

        m_UniformLocationCache[name] = location;
        return location;
    }

public:
	GLuint      m_RendererID = 0;
	std::string m_ShaderProgramName;

	std::unordered_map<std::string, GLint> m_UniformLocationCache;

    ShaderProgram() = default;
	ShaderProgram(const std::string& shaderName, const std::string& vertexShader, const std::string& fragmentShader);

	void Bind()   const { glUseProgram(m_RendererID); }
	void Unbind() const { glUseProgram(0); }

#pragma region Uniform functions

    void setBool(const std::string& name, bool value)
    {
        glUniform1i(GetUniformLocation(name), (int)value);
    }
    // ------------------------------------------------------------------------
    void setInt(const std::string& name, int value)
    {
        glUniform1i(GetUniformLocation(name), value);
    }
    // ------------------------------------------------------------------------
    void setFloat(const std::string& name, float value)
    {
        glUniform1f(GetUniformLocation(name), value);
    }
    // ------------------------------------------------------------------------
    void setVec2(const std::string& name, const glm::vec2& value)
    {
        glUniform2fv(GetUniformLocation(name), 1, &value[0]);
    }
    void setVec2(const std::string& name, float x, float y)
    {
        glUniform2f(GetUniformLocation(name), x, y);
    }
    // ------------------------------------------------------------------------
    void setVec3(const std::string& name, const glm::vec3& value)
    {
        glUniform3fv(GetUniformLocation(name), 1, &value[0]);
    }
    void setVec3(const std::string& name, float x, float y, float z)
    {
        glUniform3f(GetUniformLocation(name), x, y, z);
    }
    // ------------------------------------------------------------------------
    void setVec4(const std::string& name, const glm::vec4& value)
    {
        glUniform4fv(GetUniformLocation(name), 1, &value[0]);
    }
    void setVec4(const std::string& name, float x, float y, float z, float w)
    {
        glUniform4f(GetUniformLocation(name), x, y, z, w);
    }
    // ------------------------------------------------------------------------
    void setMat2(const std::string& name, const glm::mat2& mat)
    {
        glUniformMatrix2fv(GetUniformLocation(name), 1, GL_FALSE, &mat[0][0]);
    }
    // ------------------------------------------------------------------------
    void setMat3(const std::string& name, const glm::mat3& mat)
    {
        glUniformMatrix3fv(GetUniformLocation(name), 1, GL_FALSE, &mat[0][0]);
    }
    // ------------------------------------------------------------------------
    void setMat4(const std::string& name, const glm::mat4& mat)
    {
        glUniformMatrix4fv(GetUniformLocation(name), 1, GL_FALSE, &mat[0][0]);
    }

#pragma endregion
};

#endif

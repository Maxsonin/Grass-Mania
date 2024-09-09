#ifndef MESH
#define MESH

#include <GL/gl3w.h>

#include <vector>

#include "../Utility/Debugging.h"
#include "../Utility/OBJLoader.h"
#include "../Utility/ShaderProgram.h"

class Mesh
{
private:
    std::vector<Vertex> m_Vertices;  
    GLuint m_VAOrendererID = 0;
    GLuint m_VBOinstanceID = 0;

public:
    Mesh() = default;

    Mesh(const std::string& filepath)
    {
        OBJLoader::LoadOBJ(filepath, m_Vertices);

        // Vertex Array Initialization
        glGenVertexArrays(1, &m_VAOrendererID);
        glBindVertexArray(m_VAOrendererID);

        // Vertex Buffer Initialization
        GLuint VBOrendererID;
        glGenBuffers(1, &VBOrendererID);
        glBindBuffer(GL_ARRAY_BUFFER, VBOrendererID);
        glBufferData(GL_ARRAY_BUFFER, m_Vertices.size() * sizeof(Vertex), m_Vertices.data(), GL_STATIC_DRAW);

        // Vertex Attributes Setup
        glEnableVertexAttribArray(0); // Vertex Positions
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, position));

        glEnableVertexAttribArray(1); // Vertex Normals
        glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, normal));

        // Instance Buffer Initialization
        glGenBuffers(1, &m_VBOinstanceID);
        glBindBuffer(GL_ARRAY_BUFFER, m_VBOinstanceID);
        glBufferData(GL_ARRAY_BUFFER, 0, nullptr, GL_STREAM_DRAW); // Allocate empty buffer initially

        glEnableVertexAttribArray(2); // Instance Offset
        glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(glm::vec2), (void*)0);
        glVertexAttribDivisor(2, 1);

        // Unbind everything
        glBindVertexArray(0);
        glBindBuffer(GL_ARRAY_BUFFER, 0);

        glCheckError();
    }

    void SetInstanceData(const std::vector<glm::vec2>& instanceData)
    {
        glBindVertexArray(m_VAOrendererID);
        glBindBuffer(GL_ARRAY_BUFFER, m_VBOinstanceID);
        glBufferData(GL_ARRAY_BUFFER, instanceData.size() * sizeof(glm::vec2), instanceData.data(), GL_STREAM_DRAW);
        glBindBuffer(GL_ARRAY_BUFFER, 0);
        glCheckError();
    }

    void RenderInstanced(ShaderProgram& shaderProgram, unsigned int instanceCount)
    {
        shaderProgram.Bind(); glBindVertexArray(m_VAOrendererID);
        glDrawArraysInstanced(GL_TRIANGLES, 0, m_Vertices.size(), instanceCount);
        glCheckError();
    }
};
#endif
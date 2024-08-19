#ifndef MESH
#define MESH

#include <GL/gl3w.h>

#include <vector>

#include "OBJLoader.h"
#include "ShaderProgram.h"

class Mesh
{
private:
    std::vector<Vertex> vertices;

    GLuint m_VAOrendererID;
    GLuint m_VBOrendererID;

public:
    Mesh(const std::string& filepath)
    {
        OBJLoader loader;
        loader.LoadOBJ(filepath, vertices);

        // Vertex Array Initialization
        glGenVertexArrays(1, &m_VAOrendererID);
        glBindVertexArray(m_VAOrendererID);

        // Vertex Buffer Initialization
        glGenBuffers(1, &m_VBOrendererID);
        glBindBuffer(GL_ARRAY_BUFFER, m_VBOrendererID);
        glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(Vertex), vertices.data(), GL_STATIC_DRAW);

        // Vertex Attributes Setup
        glEnableVertexAttribArray(0); // Vertex Positions
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, position));

        glEnableVertexAttribArray(1); // Vertex Normals
        glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, normal));

        // Unbind everything
        glBindVertexArray(0);
        glBindBuffer(GL_ARRAY_BUFFER, 0);
    }

    ~Mesh()
    {
        glDeleteBuffers(1, &m_VBOrendererID);
        glDeleteVertexArrays(1, &m_VAOrendererID);
    }

    void Render(ShaderProgram& shaderProgram)
    {
        shaderProgram.Bind(); glBindVertexArray(m_VAOrendererID);
        glDrawArrays(GL_TRIANGLES, 0, vertices.size());
        glBindVertexArray(0); shaderProgram.Unbind();
    }
};

#endif

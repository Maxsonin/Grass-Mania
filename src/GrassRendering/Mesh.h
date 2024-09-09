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
    std::vector<Vertex> vertices;
    
    GLuint m_VAOrendererID = 0;

public:
    Mesh() = default;

    Mesh(const std::string& filepath)
    {
        OBJLoader::LoadOBJ(filepath, vertices);

        // Vertex Array Initialization
        glGenVertexArrays(1, &m_VAOrendererID);
        glBindVertexArray(m_VAOrendererID);

        // Vertex Buffer Initialization
        GLuint VBOrendererID;
        glGenBuffers(1, &VBOrendererID);
        glBindBuffer(GL_ARRAY_BUFFER, VBOrendererID);
        glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(Vertex), vertices.data(), GL_STATIC_DRAW);

        // Vertex Attributes Setup
        glEnableVertexAttribArray(0); // Vertex Positions
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, position));

        glEnableVertexAttribArray(1); // Vertex Normals
        glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, normal));

        // Unbind everything
        glBindVertexArray(0);
        glBindBuffer(GL_ARRAY_BUFFER, 0);

        glCheckError();
    }

    void Render(ShaderProgram& shaderProgram)
    {
        shaderProgram.Bind(); glBindVertexArray(m_VAOrendererID);
        glDrawArrays(GL_TRIANGLES, 0, vertices.size());
        glCheckError();
    }
};

#endif

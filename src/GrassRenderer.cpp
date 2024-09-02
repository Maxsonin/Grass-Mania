#include "GrassRenderer.h"

#include <cstdlib>
#include <ctime>
#include <string>
#include "Debugging.h"
#include "./Utility/Random.h"

GrassRenderer::GrassRenderer(Camera* camera)
    : m_Camera(camera)
{
    m_GrassMesh = Mesh("./resources/models/grass.obj");

    m_GrassShaderProgram = ShaderProgram("GrassShader", "./resources/shaders/GrassVertex.glsl", "./resources/shaders/GrassFragment.glsl");

    float minX = -10.0f;
    float maxX = 10.0f;
    float minZ = -10.0f;
    float maxZ = 10.0f;

    size_t grassCount = 2000;

    for (size_t i = 0; i < grassCount; i++)
    {
        float x = Random::GenerateFloat(minX, maxX);
        float z = Random::GenerateFloat(minZ, maxZ);

        m_GrassPositions.push_back(glm::vec3(x, 0.0f, z));
    }

    for (size_t i = 0; i < grassCount; i++)
    {
        m_GrassheightScaleFactor.push_back(Random::GenerateFloat(1.0f, 2.0f));
    }
}

void GrassRenderer::Render()
{
    m_GrassShaderProgram.Bind();

    m_GrassShaderProgram.setMat4("u_ViewMatrix", m_Camera->GetViewMatrix());
    m_GrassShaderProgram.setMat4("u_ProjectionMatrix", m_Camera->GetProjectionMatrix());

    m_GrassShaderProgram.setFloat("u_MinHeight", 5.0f); // Info from grass.obj
    m_GrassShaderProgram.setFloat("u_MaxHeight", 10.0f);

    for (size_t i = 0; i < 2000; i++)
    {
        glm::mat4 modelMatrix = glm::translate(glm::mat4(1.0f), m_GrassPositions[i]);
        modelMatrix = glm::scale(modelMatrix, glm::vec3(1.0f, m_GrassheightScaleFactor[i], 1.0f));
        m_GrassShaderProgram.setMat4("u_ModelMatrix", modelMatrix);

        glCheckError();
        m_GrassMesh.Render(m_GrassShaderProgram);
        glCheckError();
    }

    m_GrassShaderProgram.Unbind();
}
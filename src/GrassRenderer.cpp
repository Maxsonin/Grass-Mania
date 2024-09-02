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

    size_t grassCount = 1500;


    for (size_t i = 0; i < grassCount; i++)
    {
        float x = Random::GenerateFloat(minX, maxX);
        float z = Random::GenerateFloat(minZ, maxZ);

        m_GrassPositions.push_back(glm::vec3(x, 0.0f, z));
    }
}

void GrassRenderer::Render()
{
    m_GrassShaderProgram.Bind();

    m_GrassShaderProgram.setMat4("u_ViewMatrix", m_Camera->GetViewMatrix());
    m_GrassShaderProgram.setMat4("u_ProjectionMatrix", m_Camera->GetProjectionMatrix());

    for (const glm::vec3& position : m_GrassPositions) 
    {
        glm::mat4 modelMatrix = glm::translate(glm::mat4(1.0f), position);
        m_GrassShaderProgram.setMat4("u_ModelMatrix", modelMatrix);

        glCheckError();
        m_GrassMesh.Render(m_GrassShaderProgram);
        glCheckError();
    }

    m_GrassShaderProgram.Unbind();
}
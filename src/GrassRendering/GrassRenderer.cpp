#include "GrassRenderer.h"

#include <cstdlib>
#include <ctime>
#include <string>
#include "../Utility/Debugging.h"
#include "../Utility/Random.h"


GrassRenderer::GrassRenderer(Camera* camera)
    : m_Camera(camera)
{
    m_GrassMesh = Mesh("./resources/models/grass.obj");

    m_GrassShaderProgram = ShaderProgram("GrassShader", "./resources/shaders/GrassVertex.glsl", "./resources/shaders/GrassFragment.glsl");
    m_WorldShaderProgram = ShaderProgram("WorldShader", "./resources/shaders/WorldVertex.glsl", "./resources/shaders/WorldFragment.glsl");

    m_WorldShaderProgram.Bind();

    float plane[] =
    {
        0.0f,  0.0f, 0.0f,
        0.0f,  0.0f, 10.0f,
        10.0f, 0.0f, 10.0f,

        10.0f, 0.0f, 10.0f,
        10.0f,  0.0f, 0.0f,
        0.0f,  0.0f, 0.0f
    };

    GLuint worldVBO = 0;
    glGenBuffers(1, &worldVBO);
    glBindBuffer(GL_ARRAY_BUFFER, worldVBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(plane), plane, GL_STATIC_DRAW);

    glGenVertexArrays(1, &worldVAO);
    glBindVertexArray(worldVAO);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
    glBindVertexArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, 0);

    glCheckError();

    float minX = 0.0f;
    float maxX = 150.0f;
    float minZ = 0.0f;
    float maxZ = 150.0f;

    grassCount = 1500 * maxX / 10;

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

void GrassRenderer::Render(Camera* debugCam = nullptr)
{
    frustum = CreateFrustumOfCamera(*m_Camera, 1.0f);

    // Render the plane first
    m_WorldShaderProgram.Bind();

    glm::mat4 planeModelMatrix = glm::scale(glm::mat4(1.0f), glm::vec3(15.0f, 1.0f, 15.0f));
    m_WorldShaderProgram.setMat4("u_ModelMatrix", planeModelMatrix);
    if (debugCam != nullptr && debugCam->m_IsMain)
    {
        m_WorldShaderProgram.setMat4("u_ViewMatrix", debugCam->GetViewMatrix());
        m_WorldShaderProgram.setMat4("u_ProjectionMatrix", debugCam->m_ProjectionMatrix);
    }
    else
    {
        m_WorldShaderProgram.setMat4("u_ViewMatrix", m_Camera->GetViewMatrix());
        m_WorldShaderProgram.setMat4("u_ProjectionMatrix", m_Camera->m_ProjectionMatrix);
    }

    glBindVertexArray(worldVAO);
    glDrawArrays(GL_TRIANGLES, 0, 6);
    glBindVertexArray(0);
    m_WorldShaderProgram.Unbind();

    glCheckError();

    m_GrassShaderProgram.Bind();

    if (debugCam != nullptr && debugCam->m_IsMain)
    {
        m_GrassShaderProgram.setMat4("u_ViewMatrix", debugCam->GetViewMatrix());
        m_GrassShaderProgram.setMat4("u_ProjectionMatrix", debugCam->m_ProjectionMatrix);
    }
    else
    {
        m_GrassShaderProgram.setMat4("u_ViewMatrix", m_Camera->GetViewMatrix());
        m_GrassShaderProgram.setMat4("u_ProjectionMatrix", m_Camera->m_ProjectionMatrix);
    }

    m_GrassShaderProgram.setFloat("u_MinHeight", 5.0f); // Info from grass.obj
    m_GrassShaderProgram.setFloat("u_MaxHeight", 10.0f);

    for (size_t i = 0; i < grassCount; i++)
    {
        glm::vec3 grassPosition = m_GrassPositions[i];
        float scaleY = m_GrassheightScaleFactor[i];

        // Create AABB for the grass instance
        glm::vec3 minPoint = grassPosition - glm::vec3(0.1f, 0.0f, 0.1f);
        glm::vec3 maxPoint = grassPosition + glm::vec3(0.1f, scaleY, 0.1f);
        AABB grassAABB(minPoint, maxPoint);

        // Perform frustum culling
        if (!grassAABB.isOnFrustum(frustum))
        {
            continue; // Skip rendering this grass mesh if it's outside the frustum
        }

        glm::mat4 modelMatrix = glm::translate(glm::mat4(1.0f), grassPosition);
        modelMatrix = glm::scale(modelMatrix, glm::vec3(1.0f, scaleY, 1.0f));
        m_GrassShaderProgram.setMat4("u_ModelMatrix", modelMatrix);

        glCheckError();
        m_GrassMesh.Render(m_GrassShaderProgram);
        glCheckError();
    }

    m_GrassShaderProgram.Unbind();
}